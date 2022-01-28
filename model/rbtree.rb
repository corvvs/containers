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
      if key == node.key
        break
      elsif key < node.key
        node = node.left
      elsif node.key < key
        node = node.right
      end
    end
    node
  end

  def add(key)
    if !@end.left
      @end.left = @end.add_left(key)
      return @end.left
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
          return node.parent.add_left(key)
        else
          p "added #{key} to right of #{node.parent.key}:#{node.parent.type}"
          return node.parent.add_right(key)
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
end

tree = BinTree.new
items = (1..5).to_a.shuffle
p items
items.each{ |v| tree.add(v) }

p tree.root.min.key
p tree.root.max.key

puts "-- normal forward --"
it = tree.it_begin
while true do
  p [it.node.key, it.node.min.key, it.node.max.key]
  p tree.constraint_black_or_white
  p tree.constraint_root_is_black
  p tree.constraint_all_leaves_are_black
  p tree.constraint_not_red_red
  p tree.constraint_same_black_height
  p tree.root.min.key
  p tree.root.max.key
  break if it.node == tree.it_end.node
  it = it.next
end
puts "-- normal backward --"
while true  do
  p [it.node.key, it.node.min.key, it.node.max.key]
  break if it.node == tree.it_begin.node
  it = it.prev
end
