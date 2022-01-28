class BinTree
  class Node
    attr_reader   :key, :is_end, :type
    attr_accessor :left, :right, :parent, :color
    def initialize(key, parent, color, type = :normal)
      # [type]
      # - :normal   通常ノード
      # - :end      endノード
      # - :nil      NILノード
      @type = type
      @key = key
      @is_end = type == :end
      @parent = parent
      @color = color
      if @type == :nil
        @left = nil
        @right = nil
      else
        @left = Node.new(nil, self, :B, :nil)
        if @type == :end
          @right = nil
        else
          @right = Node.new(nil, self, :B, :nil)
        end
      end
    end

    def flip_color
      if @type != :normal
        fail "tried to flip_color non-normal node"
      end
      @color = @color == :B ? :R : :B
      p "flipped color of #{@key} -> #{@color}"
    end

    def add_left(key)
      added = Node.new(key, self, :R)
      self.left = added
      added
    end

    def add_right(key)
      added = Node.new(key, self, :R)
      self.right = added
      added
    end

    # 自身を根とする部分木の最大ノードを返す
    def max
      if !has_right_child
        return self
      end
      node = @right
      while node.has_right_child do
        node = node.right
      end
      return node
    end

    # 自身を根とする部分木の最小ノードを返す
    def min
      if !has_left_child
        return self
      end
      node = @left
      while node.has_left_child do
        node = node.left
      end
      return node
    end

    def is_normal_node
      @type == :normal
    end

    # 自身が左子かどうかを返す
    # ルートは左子ではない(特別扱い)
    def is_left_child
      return false if !@parent
      return false if !@parent.left
      return false if @parent.key == :end
      return @parent.left == self
    end

    # 自身が右子かどうかを返す
    # ルートは右子ではない(特別扱い)
    def is_right_child
      return false if !@parent
      return false if !@parent.right
      return false if @parent.key == :end
      return @parent.right == self
    end

    def has_left_child
      @left && @left.type != :nil
    end

    def has_right_child
      @right && @right.type != :nil
    end

    # 自身に後隣接するノードを返す
    def forward_neighbor
      if @right.is_normal_node
        return @right.min
      end
      if @type == :end
        return @left.min
      end
      node = self
      while node.is_right_child
        node = node.parent
      end
      return node.parent
    end

    # 自身に前隣接するノードを返す
    def backward_neighbor
      if @left.is_normal_node
        return @left.max
      end
      if @type == :end
        return @right.max
      end
      node = self
      while node.is_left_child
        node = node.parent
      end
      return node.parent
    end

    def set_key(key)
      @key = key
    end

    def is_black_or_white
      if @color != :B && @color != :R
        return false
      end
      (!@left || @left.is_black_or_white) && (!@right || @right.is_black_or_white)
    end

    def leave_is_black
      if @left && !@left.leave_is_black
        return false
      end
      if @right && !@right.leave_is_black
        return false
      end
      # 葉の場合
      if !@left && !@right
        if @type != :nil
          p "leave is not :nil"
        end
        return @color == :B
      end
      return true
    end

    def not_red_red
      if @color == :R
        if @left && @left.color == :R
          return false
        end
        if @right && @right.color == :R
          return false
        end
      end
      return (!@left || @left.not_red_red) && (!@right || @right.not_red_red)
    end

    def black_height
      n = @color == :B ? 1 : 0
      nl = @left ? @left.black_height : 0
      nr = @right ? @right.black_height : 0
      if nl < 0 || nr < 0 || nl != nr
        return -1
      end
      n + nr
    end

    def rotate(child)
      if @left == child
        rotate_right
      elsif @right == child
        rotate_left
      else
        fail "tried to rotate a pair not in parent-child: #{@key} - #{child.key}"
      end
    end

    def rotate_left
      if !@right
        fail "tried to rotate left with no right child #{@key}"
      end
      r = @right
      p "L-rot #{@key} - #{r.key}"
      rr = r.left
      pa = @parent
      lchild = is_left_child
      @right = rr
      rr.parent = self
      r.left = self
      self.parent = r
      if lchild
        pa.left = r
      else
        pa.right = r
      end
      r.parent = pa
    end

    def rotate_right
      if !@left
        fail "tried to rotate left with no right child #{@key}"
      end
      r = @left
      p "R-rot #{@key} - #{r.key}"
      rr = r.right
      pa = @parent
      lchild = is_left_child
      @left = rr
      rr.parent = self
      r.right = self
      self.parent = r
      if lchild
        pa.left = r
      else
        pa.right = r
      end
      r.parent = pa
    end

    def counter_child(child)
      if @left == child
        return @right
      elsif @right == child
        return @left
      end
      fail "#{child.key} is not a child of #{@key}"
    end

  end


  # 二分木のイテレータ
  class Iterator
    attr_reader :tree, :node
    def initialize(tree, node, is_forward = true)
      @tree = tree
      @node = node
      @is_forward = is_forward
    end

    def next
      _shift(true == @is_forward, nil)
    end

    def prev
      _shift(false == @is_forward, nil)
    end

    def _shift(forward, from)
      from = from || @node
      to = @node

      if forward
        # 後隣接を求める場合
        to = @node.forward_neighbor
      else
        # 前隣接を求める場合
        to = @node.backward_neighbor
      end
      return Iterator.new(@tree, to, @is_forward)
    end
  end

  attr_reader     :end
  def initialize
    @end = Node.new(nil, nil, :B, :end)
  end

  def root
    return @end.left
  end

  # keyを持つノードを返す
  # なければnilを返す
  def find(key)
    node = root
    while node do
      if node.type != :normal || key == node.key
        break
      elsif key < node.key
        node = node.left
      elsif node.key < key
        node = node.right
      end
    end
    node
  end

  def find_count(key)
    node = root
    n = 0
    while node do
      if node.type != :normal || key == node.key
        break
      elsif key < node.key
        node = node.left
      elsif node.key < key
        node = node.right
      end
      n += 1
    end
    node.type == :normal ? n : nil
  end

  def add(key)
    if !@end.left
      @end.left = @end.add_left(key)
      return rebalance(@end.left)
    end
    # 普通の二分木のadd
    node = root
    while node do
      p [node.key, node.type, node.parent.has_left_child]
      if node.type == :nil
        # NILノード
        # -> ここに追加
        if node.parent.left == node
          p "added #{key} to left of #{node.parent.key}:#{node.parent.type}"
          return rebalance(node.parent.add_left(key))
        else
          p "added #{key} to right of #{node.parent.key}:#{node.parent.type}"
          return rebalance(node.parent.add_right(key))
        end
      end
      if key < node.key
        # 追加キーがノードよりも小さい
        node = node.left
      elsif node.key < key
        node = node.right
      else
        # 重複なし二分木ならなにもしない
        p "not added"
        return node
      end
    end
  end

  # ノードの削除
  def delete(key, from = nil)
    node = from || find(key)
    if !node
      return nil
    end
    if node.right
      # 右子(右部分木)をもつ
      # -> 右部分木の最小値のkeyをもってくる
      # -> 右部分木の最小値を削除する
      submin = node.right.min
      node.set_key(submin.key)
      return delete(node.key, submin)
    end
    if node.left
      # 左子を持つ
      # -> TのparentとTの左子を接続して終了
      parent = node.parent
      if node.is_right_child
        parent.right = node.left
        node.left.parent = parent
      else
        parent.left = node.left
        node.left.parent = parent
      end
      node.parent = nil
      node.left = nil
      return key
    end
    # Tが子を持たない場合
    parent = node.parent
    if node.is_right_child
      parent.right = nil
    else
      parent.left = nil
    end
    node.parent = nil
    return key
  end

  def it_begin()
    it = Iterator.new(self, root.min)
  end

  def it_rbegin()
    it = Iterator.new(self, root.max, false)
  end

  def it_end()
    Iterator.new(self, @end)
  end

  def it_rend()
    Iterator.new(self, @end)
  end

  # 制約のチェック
  def constraint_black_or_white
    root.is_black_or_white
  end

  # 2. ルート制約
  def constraint_root_is_black
    root.color == :B
  end

  # 3. 葉がすべて黒
  def constraint_all_leaves_are_black
    root.leave_is_black
  end

  # 4. 赤制約
  def constraint_not_red_red
    root.not_red_red
  end

  # 5. 黒制約
  def constraint_same_black_height
    root.black_height >= 0
  end

  # リバランス
  def rebalance(n)
    p "rebalancing #{n.key}"
    # 0. Nが通常ノードでないか、黒ノードの場合
    # -> なにもしない
    if !n.is_normal_node || n.color == :B
      p "do nothing(n is B)"
      return n
    end
    # 1. Nが根ノードの場合
    # -> 色変して終わり
    if n == root
      p "changed #{n.key} -> B"
      n.flip_color
      return n
    end
    # 2. Nの親が黒ノードの場合
    # -> なにもしなくてよい
    if n.parent.color == :B
      p "do nothing(parent is B)"
      return n
    end
    
    # 3. Nの親が赤ノードの場合
    if n.is_left_child != n.parent.is_left_child
      # 曲がっている場合(シス)
      # -> Nの親の親とNの親で回転させる(トランスにする)
      # https://ja.wikipedia.org/wiki/%E3%82%B7%E3%82%B9_(%E5%8C%96%E5%AD%A6)
      p "detected cis: #{n.key} - #{n.parent.key} - #{n.parent.parent.key}"
      pa = n.parent
      pa.rotate(n)
      n = pa
    end

    pa = n.parent
    q = pa.parent
    u = q.counter_child(pa)
    p "n = #{n.key}, p = #{pa.key}, q = #{q.key}, u = #{u.key}"
    if u.color == :B
      # Uが黒ノードの場合
      # P,Qを色変する
      pa.flip_color
      q.flip_color
      # さらにQ-Pに対して右回転を行う。
      q.rotate(pa)
      return n
    else
      # Uが赤ノードの場合
      # P, Q, Uの色を反転する。
      pa.flip_color
      q.flip_color
      u.flip_color
      # Q -> N として、再帰的に処理を繰り返すことにする。
      return rebalance(q)
    end

  end
end


def all_constraint(tree)
  [
    :constraint_black_or_white,
    :constraint_root_is_black,
    :constraint_all_leaves_are_black,
    :constraint_not_red_red,
    :constraint_same_black_height,
  ].each{ |sym|
    if !tree.send(sym)
      fail "failed at constraint #{sym}"
    end
  }
end

tree = BinTree.new
items = (1..500).to_a.shuffle
#items = [5, 2, 3, 1, 4]
p items
items.each{ |v| 
  p "adding #{v}"
  tree.add(v)
  all_constraint(tree)
}


puts "-- random sampling --"
samples = items.size / 20
samples.times{
  key = items.sample(1)[0]
  n = tree.find_count(key)
  ln = Math.log2(items.size.to_f)
  puts "found #{key} in depth #{n} / #{ln}"
}

#exit

puts "-- normal forward --"
is = items.sort
it = tree.it_begin
qs = []
while true do
  # p [it.node.key, it.node.min.key, it.node.max.key]
  all_constraint(tree)
  break if it.node == tree.it_end.node
  qs << it.node.key
  it = it.next
end
#p qs
fail "not sorted" if is != qs
puts "-- normal backward --"
qs = []
while true  do
  # p [it.node.key, it.node.min.key, it.node.max.key]
  break if it.node == tree.it_begin.node
  it = it.prev
  qs << it.node.key
end
qs.reverse!
#p qs
fail "not sorted" if is != qs
