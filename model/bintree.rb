class BinTree
  class Node
    attr_reader   :key, :is_end
    attr_accessor :left, :right, :parent
    def initialize(key, parent)
      @key = key
      @is_end = !key
      @parent = parent
      @left = nil
      @right = nil
    end

    def add_left(key)
      added = Node.new(key, self)
      self.left = added
      added
    end

    def add_right(key)
      added = Node.new(key, self)
      self.right = added
      added
    end

    # 自身を根とする部分木の最大ノードを返す
    def max
      if !@right
        return self
      end
      node = @right
      while node.right do
        node = node.right
      end
      return node
    end

    # 自身を根とする部分木の最小ノードを返す
    def min
      if !@left
        return self
      end
      node = @left
      while node.left do
        node = node.left
      end
      return node
    end

    # 自身が左子かどうかを返す
    # ルートは左子ではない(特別扱い)
    def is_left_child
      return false if !@parent
      return false if !@parent.left
      return false if !@parent.key
      return @parent.left.key == @key
    end

    # 自身が右子かどうかを返す
    # ルートは右子ではない(特別扱い)
    def is_right_child
      return false if !@parent
      return false if !@parent.right
      return false if !@parent.key
      return @parent.right.key == @key
    end

    # 自身に後隣接するノードを返す
    def forward_neighbor
      if @right
        return @right.min
      end
      if !@key
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
      if @left
        return @left.max
      end
      if !@key
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
    @end = Node.new(nil, nil)
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
      if key < node.key
        # 追加キーがノードよりも小さい
        if node.left
          # 左子があれば左に降りる
          node = node.left
        else
          # 左子がなければ左子に追加して終了
          node.left = node.add_left(key)
          return node.left
        end
      elsif node.key < key
        # 追加キーがノードよりも大きい
        if node.right
          # 右子があれば右に降りる
          node = node.right
        else
          # 右子がなければ右子に追加
          node.right = node.add_right(key)
          return node.right
        end
      else
        # 重複なし二分木ならなにもしない
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
end

tree = BinTree.new
items = (1..10).to_a.shuffle
# items = [5, 3, 4, 6, 10, 8, 9, 2, 1, 7]
p items
items.each{ |v| tree.add(v) }

# p tree.root.min.key
# p tree.root.max.key
# p tree.root.min.is_left_child
# p tree.root.min.is_right_child
# p tree.root.max.is_left_child
# p tree.root.max.is_right_child

# items.shuffle.each{ |key| tree.delete(key) }
tree.delete(items[0])

puts "-- normal forward --"
it = tree.it_begin
while true do
  p [it.node.key, it.node.min.key, it.node.max.key]
  break if it.node == tree.it_end.node
  it = it.next
end
puts "-- normal backward --"
while true do
  p [it.node.key, it.node.min.key, it.node.max.key]
  break if it.node == tree.it_begin.node
  it = it.prev
end

puts "-- reverse forward --"
it = tree.it_rbegin
while true do
  p [it.node.key, it.node.min.key, it.node.max.key]
  break if it.node == tree.it_rend.node
  it = it.next
end

puts "-- reverse backward --"
while true do
  p [it.node.key, it.node.min.key, it.node.max.key]
  break if it.node == tree.it_rbegin.node
  it = it.prev
end
