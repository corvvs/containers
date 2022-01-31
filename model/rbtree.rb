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
      @left = nil
      @right = nil
      if @type != :nil
        @left = Node.new(nil, self, :B, :nil)
        if @type != :end
          @right = Node.new(nil, self, :B, :nil)
        end
      end
    end

    # selfを親ノードとしてNILノードを作る
    def spawn_nil
      return Node.new(nil, self, :B, :nil)
    end

    def flip_color
      if @type != :normal
        fail "tried to flip_color non-normal node"
      end
      @color = @color == :B ? :R : :B
      #p "flipped color of #{@key} -> #{@color}"
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

    def is_leaf
      @type == :nil
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
      self.left && self.left.type != :nil
    end

    def has_right_child
      self.right && self.right.type != :nil
    end

    # 右の部分木を持つかどうか
    def has_right_subtree
      self.right && self.right.type != :nil
    end

    # 左の部分木を持つかどうか
    def has_left_subtree
      self.left && self.left.type != :nil
    end

    # 自身を根とする部分木の最小ノードを返す
    def subtree_minimum
      if has_left_subtree
        return self.left.subtree_minimum
      end
      return self
    end

    def signiture
      "#{@key}-#{@color}-#{@type}"
    end

    # 左子を削除し、葉ノードに置き換える
    def delete_left
      #p "delete_left: of #{self.signiture}"
      l = self.left
      return if l.is_leaf
      l.parent = nil
      self.left = self.spawn_nil
    end

    # 右子を削除し、葉ノードに置き換える
    def delete_right
      #p "delete_right: of #{self.signiture}"
      r = self.right
      #p "r is leaf?: #{r.is_leaf}"
      return if r.is_leaf
      r.parent = nil
      self.right = self.spawn_nil
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

    def set_type(type)
      @type = type
    end

    def is_root
      return self.parent && self.parent.type == :end
    end

    def is_black_or_white
      if @color != :B && @color != :R
        fail "#{self.key}-#{self.type} is not B or R: #{self.color}"
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
          #p "leave is not :nil"
        end
        return @color == :B
      end
      return true
    end

    def not_red_red
      if @color == :R
        if @left && @left.color == :R
          fail "#{self.key}-#{self.type} and it's left child #{self.left.key}-#{self.left.type} is red"
        end
        if @right && @right.color == :R
          fail "#{self.key}-#{self.type} and it's right child #{self.right.key}-#{self.right.type} is red"
        end
      end
      return (!@left || @left.not_red_red) && (!@right || @right.not_red_red)
    end

    def black_height
      n = @color == :B ? 1 : 0
      nl = @left ? @left.black_height : 0
      nr = @right ? @right.black_height : 0
      if nl < 0 || nr < 0 || nl != nr
        fail "black-height not matched at #{@key}-#{@color}-#{@type}: #{nl} #{nr}"
      end
      h = n + nr
      #p "black-height at #{@key}-#{@color}-#{@type} is #{h} / #{nl} / #{nr}"
      h
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

    # rotateの後、回転に関わるノードをflip_colorする
    def rotate_flip(child)
      #puts "rotate_flip: #{self.key}-#{self.color} : #{child.key}-#{child.color}"
      self.rotate(child)
      self.flip_color
      child.flip_color
    end

    # rotateの後、回転に関わるノードの色を交換する
    def rotate_swap(child)
      #puts "rotate_swap: #{self.signiture} : #{child.signiture}"
      self.rotate(child)
      color = self.color
      self.color = child.color
      child.color = color
    end

    def rotate_left
      if !@right
        fail "tried to rotate left with no right child #{@key}"
      end
      r = @right
      #p "L-rot #{@key} - #{r.key}"
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
      #p "R-rot #{@key} - #{r.key}"
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

    # 子のうち、親ノードと同じ側にあるものを返す
    def cis_child
      if self.is_left_child
        return self.right
      elsif self.is_right_child
        return self.left
      end
      fail "no parent"
    end

    # 子のうち、親ノードと逆側にあるものを返す
    def trans_child
      if self.is_left_child
        return self.left
      elsif self.is_right_child
        return self.right
      end
      fail "no parent"
    end

    # ノード削除
    def delete_self
      # puts "delete_self: #{self.key} - #{self.type} - #{self.color}"
      # self = T
      if self.has_right_subtree
        #p "has right subtree"
        # Tが右部分木を持つ
        # -> Tの右部分木の最小値をTと読み替える
        nm = @right.subtree_minimum
        self.set_key(nm.key)
        nm.delete_self
        return
      end

      if self.has_left_subtree
        #p "has left subtree"
        # Tが左部分木を持つ
        # -> 左子の値をTにコピーし、左子を削除
        self.set_key(self.left.key)
        self.left.delete_self
        return
      end

      if self.color == :R
        #puts "Case 0. is red"
        # Tが赤である
        # -> Case 0 Tを葉ノードにする
        if self.is_left_child
          self.parent.delete_left
        else
          self.parent.delete_right
        end
        return
      end

      # Tは右部分木も左部分木も持たず、黒である。
      # M = 右子の値をTにコピーする
      # Mを削除する
      # TをMと読み替える
      np = self.parent
      m = nil
      if self.is_left_child
        np.delete_left
        m = np.left
      elsif self.is_right_child
        np.delete_right
        m = np.right
      else
        fail "self is not a right child"
      end
      # Mに対して黒リバランスを行う
      m.rebalance_black
    end

    # 黒リバランス
    def rebalance_black
      #puts "rebalance_black: #{self.key} - #{self.type} - #{self.color}"
      # self = M
      if self.is_root
        #puts "Case 1. is_root"
        # Mは根である
        # -> Case 1.
        # なにもしない
        return
      end

      nm = self
      np = nm.parent
      ns = np.counter_child(nm)
      nx = ns.cis_child
      ny = ns.trans_child
      if np.color == :B && ns.color == :B && nx.color == :B && ny.color == :B
        #puts "Case 3. BBBB"
        # PSXYすべて黒
        # BBBB
        # Sを赤に色変
        ns.flip_color
        # Pを基準にしてもう一度黒リバランスを行う
        np.rebalance_black
        return
      end

      if np.color == :B && ns.color == :R && nx.color == :B && ny.color == :B
        #puts "Case 2. BSBB"
        #puts "p: #{np.signiture} s: #{ns.signiture} x: #{nx.signiture} y: #{ny.signiture}"
        # PXYが黒, Sが赤
        # BSBB
        np.rotate_flip(ns)
        np = nm.parent
        ns = np.counter_child(nm)
        nx = ns.cis_child
        ny = ns.trans_child
        #puts "p: #{np.signiture} s: #{ns.signiture} x: #{nx.signiture} y: #{ny.signiture}"
      end
      if np.color == :R && ns.color == :B && nx.color == :B && ny.color == :B
        #puts "Case 4. RBBB"
        # SXYが黒, Pが赤
        # RBBB
        np.flip_color
        ns.flip_color
        return
      end

      if ns.color == :B && nx.color == :R && ny.color == :B
        # SYが黒, Xが赤
        # ?BRB
        # -> Case 5.
        #puts "Case 5. ?BRB"
        #puts "p: #{np.signiture} s: #{ns.signiture} x: #{nx.signiture} y: #{ny.signiture}"
        ns.rotate_flip(nx)
        np = nm.parent
        ns = np.counter_child(nm)
        nx = ns.cis_child
        ny = ns.trans_child
      end
      #puts "Case 6. ?B?R"
      # ?B?R
      #puts "p: #{np.signiture} s: #{ns.signiture} x: #{nx.signiture} y: #{ny.signiture}"
      np.rotate_swap(ns)
      ny.flip_color
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
      if node.type != :normal
        return nil
      elsif key == node.key
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
      return rebalance_red(@end.left)
    end
    # 普通の二分木のadd
    node = root
    while node do
      # p [node.key, node.type, node.parent.has_left_child]
      if node.type == :nil
        # NILノード
        # -> ここに追加
        if node.parent.left == node
          #p "added #{key} to left of #{node.parent.key}:#{node.parent.type}"
          return rebalance_red(node.parent.add_left(key))
        else
          #p "added #{key} to right of #{node.parent.key}:#{node.parent.type}"
          return rebalance_red(node.parent.add_right(key))
        end
      end
      if key < node.key
        # 追加キーがノードよりも小さい
        node = node.left
      elsif node.key < key
        node = node.right
      else
        # 重複なし二分木ならなにもしない
        #p "not added"
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
    it = root.is_leaf ? self.it_end : Iterator.new(self, root.min)
  end

  def it_rbegin()
    it = root.is_leaf ? self.it_rend : Iterator.new(self, root.max, false)
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
  def rebalance_red(n)
    #p "rebalancing #{n.key}"
    # 0. Nが通常ノードでないか、黒ノードの場合
    # -> なにもしない
    if !n.is_normal_node || n.color == :B
      #p "do nothing(n is B)"
      return n
    end
    # 1. Nが根ノードの場合
    # -> 色変して終わり
    if n == root
      n.flip_color
      return n
    end
    # 2. Nの親が黒ノードの場合
    # -> なにもしなくてよい
    if n.parent.color == :B
      #p "do nothing(parent is B)"
      return n
    end
    
    # 3. Nの親が赤ノードの場合
    if n.is_left_child != n.parent.is_left_child
      # 曲がっている場合(シス)
      # -> Nの親の親とNの親で回転させる(トランスにする)
      # https://ja.wikipedia.org/wiki/%E3%82%B7%E3%82%B9_(%E5%8C%96%E5%AD%A6)
      #p "detected cis: #{n.key} - #{n.parent.key} - #{n.parent.parent.key}"
      pa = n.parent
      pa.rotate(n)
      n = pa
    end

    pa = n.parent
    q = pa.parent
    u = q.counter_child(pa)
    #p "n = #{n.key}, p = #{pa.key}, q = #{q.key}, u = #{u.key}"
    if u.color == :B
      # Uが黒ノードの場合
      # P,Qを色変する
      pa.flip_color
      q.flip_color
      # さらにQ-Pに対して右回転を行う。
      q.rotate(pa)
      return
    else
      # Uが赤ノードの場合
      # P, Q, Uの色を反転する。
      pa.flip_color
      q.flip_color
      u.flip_color
      # Q -> N として、再帰的に処理を繰り返すことにする。
      rebalance_red(q)
      return
    end

  end

  # ノード削除
  def delete(key)
    node = find(key)
    return if !node

    node.delete_self
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

def traverse_tree(tree)
  tit = tree.it_begin
  arr = []
  while tit.node != tree.it_end.node do
    arr << "#{tit.node.key}-#{tit.node.color}"
    tit = tit.next
  end
  p arr
end

def clock
  t0 = Time.now.to_f
  yield
  t1 = Time.now.to_f
  return (t1 - t0)
end

# ------------------------------------------


def test_add
  tree = BinTree.new
  items = (1..5000).to_a.reverse
  #items = [5, 2, 3, 1, 4]
  p items
  items.each{ |v| 
    # p "adding #{v}"
    tree.add(v)
    all_constraint(tree)
  }


  puts "-- random sampling --"
  samples = items.size / 5
  nmin = 100000000000
  nmax = -100000000000
  samples.times{
    key = items.sample(1)[0]
    n = tree.find_count(key)
    nmin = n if n < nmin
    nmax = n if n > nmax
    puts "found #{key} in depth #{n}"
  }
  ln = Math.log2(items.size.to_f)
  p [nmin, nmax, ln]

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
end

def test_delete
  # ルートのみの木で、ルートを削除する
  def test_delete_root_only_root
    tree = BinTree.new
    all_constraint(tree)
    tree.add(1)
    all_constraint(tree)
    tree.delete(1)
    all_constraint(tree)
  end

  # ノードが2つのみの木で、ルートを削除する
  def test_delete_root_2_nodes
    tree = BinTree.new
    all_constraint(tree)
    tree.add(1)
    p tree.find(1).has_left_subtree
    p tree.find(1).has_right_subtree
    tree.add(2)
    p tree.find(1).has_left_subtree
    p tree.find(1).has_right_subtree
    all_constraint(tree)
    traverse_tree(tree)
    tree.delete(1)
    all_constraint(tree)
    traverse_tree(tree)
  end

  def test_delete_case_6
    tree = BinTree.new
    all_constraint(tree)
    [3, 2, 7, 5, 8, 1, 4, 6].each{ |key| tree.add(key) }
    all_constraint(tree)
    traverse_tree(tree)
    tree.delete(8)
    traverse_tree(tree)
    all_constraint(tree)
  end

  def test_delete_case_4
    tree = BinTree.new
    all_constraint(tree)
    [3, 2, 7, 5, 8, 1, 4, 6].each{ |key| tree.add(key) }
    all_constraint(tree)
    traverse_tree(tree)
    tree.delete(4)
    tree.delete(6)
    traverse_tree(tree)
    all_constraint(tree)
    tree.delete(5)
    traverse_tree(tree)
    all_constraint(tree)
  end

  def test_delete_case_5
    tree = BinTree.new
    tree.add(3)
    tree.add(2)
    tree.add(5)
    traverse_tree(tree)
    tree.add(1)
    traverse_tree(tree)
    tree.add(4)
    traverse_tree(tree)
    tree.delete(1)
    traverse_tree(tree)
    all_constraint(tree)
    tree.delete(2)
    traverse_tree(tree)
    all_constraint(tree)
    tree.delete(3)
    traverse_tree(tree)
    all_constraint(tree)
    tree.delete(4)
    traverse_tree(tree)
    all_constraint(tree)
    tree.delete(5)
    traverse_tree(tree)
    all_constraint(tree)
  end

  def test_delete_random
    s = 10
    t = 1000
    n = (Math.exp(rand() * Math.log(t / s)) * s).to_i
    $stderr.print "#{n} "
    dt = clock {
      tree = BinTree.new
      ns = ((1..n).to_a * 2).shuffle
      vs = {}
      items = ns.map{ |n|
        if vs[n]
          next [n, :delete]
        end
        vs[n] = 1
        [n, :add]
      }
      p items
      items.each{ |key, action|
        tree.send(action, key)
        all_constraint(tree)
        traverse_tree(tree)
      }
    }
    $stderr.puts sprintf("%1.4f", dt * 1000)
  end

  def test_sample_delete(n)
    tree = BinTree.new
    items = (1..n).to_a.shuffle
    items.each{ |k|
      dt = clock {
        tree.add(k)
      }
    }
    all_constraint(tree)
    dts = []
    m = 40
    items.sample(m).each{ |k|
      dts << clock {
        tree.delete(k)
      }
    }
    dts.sort!
    mm = dts.size / 2
    adt = dts[dts.size / 4 .. (dts.size / 4 + mm)].sum / mm
    $stderr.puts sprintf("%d\t%1.4f", n, adt * 1000000)
    all_constraint(tree)
  end

  # test_delete_root_only_root
  # test_delete_root_2_nodes
  # test_delete_case_6
  # test_delete_case_4
  # test_delete_case_5
  1000.times {
    s = 100
    t = 10000000
    n = (Math.exp(rand() * Math.log(t / s)) * s).to_i
    test_sample_delete(n)
  }
end


#test_add
test_delete
