class Color:
  RED = 0
  BLACK = 1

class Node:
  def __init__(self, nums, data):
    self.num = nums
    self.data = data
    
    self.color = Color.RED
    self.height = 1

    self.left = None
    self.right = None
    self.parent = None
  
  def GetColor(self):
    if (self.color == Color.RED): return 'R'
    else: return 'B'

class RedBlackTree:
  def __init__(self):
    self.upper = None
    self.recent = None
  ##################################################
  def __Rotate(self, oldRoot, code):
    
    parents = oldRoot.parent
    parto = 'L'
    if (parents and parents.left != oldRoot): parto = 'R'
    newRoot = None
    temp = None 
    
    if (code == 'L'):
      newRoot = oldRoot.right
      temp = newRoot.left
      newRoot.left = oldRoot
      oldRoot.right = temp
    elif (code == 'R'):
      newRoot = oldRoot.left
      temp = newRoot.right
      newRoot.right = oldRoot
      oldRoot.left = temp
    
    if (temp): temp.parent = oldRoot
    oldRoot.parent = newRoot
    newRoot.parent = parents
    if (parents): 
      if (parto == 'L'): parents.left = newRoot
      elif (parto == 'R'): parents.right = newRoot
    else: self.upper = newRoot

    return newRoot
  
  ##############################################
  def __FixPush(self, root):
    if not (root.parent):
      self.upper = root
      self.upper.color = Color.BLACK
      return
    elif not (root.parent.parent):
      self.__FixPush(root.parent)
      return
    NodeGP = root.parent.parent
    NodeP = root.parent
    NodeU = None

    if (root.color == Color.BLACK or NodeP.color == Color.BLACK): return

    if (NodeGP.left and NodeGP.left == root.parent):
      NodeU = NodeGP.right
      
      if (NodeU and NodeU.color == Color.RED):
        NodeGP.color = Color.RED
        if (NodeGP.left): NodeGP.left.color = Color.BLACK
        if (NodeGP.right): NodeGP.right.color = Color.BLACK
      
      else:
        if (NodeGP.left.right == root): NodeP = self.__Rotate(NodeP, 'L')
        NodeGP = self.__Rotate(NodeGP, 'R')
        NodeGP.color = Color.BLACK
        if (NodeGP.left): NodeGP.left.color = Color.RED
        if (NodeGP.right): NodeGP.right.color = Color.RED

    elif (NodeGP.right and NodeGP.right == root.parent):
      NodeU = NodeGP.left
      
      if (NodeU and NodeU.color == Color.RED):
        NodeGP.color = Color.RED
        if (NodeGP.left): NodeGP.left.color = Color.BLACK
        if (NodeGP.right): NodeGP.right.color = Color.BLACK

      else:
        if (NodeGP.right.left == root): NodeP = self.__Rotate(NodeP, 'R')
        NodeGP = self.__Rotate(NodeGP, 'L')
        NodeGP.color = Color.BLACK
        if (NodeGP.left): NodeGP.left.color = Color.RED
        if (NodeGP.right): NodeGP.right.color = Color.RED

    return self.__FixPush(NodeGP)

  def __PushProcess(self, root, num, data):
    if (root == None):
      self.recent = Node(num, data)
      return self.recent
    elif (num < root.num):
      root.left = self.__PushProcess(root.left, num, data)
      root.left.parent = root
    elif (num > root.num):
      root.right = self.__PushProcess(root.right, num, data)
      root.right.parent = root
    return root

  ##############################################
  def __PrintTree(self, root, s):
    if (root):
      self.__PrintTree(root.right, s+6)
      a = root.parent.num if root.parent else "OOF"
      print(f"{' '*s}{root.num}-{root.GetColor()}-{a}", end="\n")
      self.__PrintTree(root.left, s+6)
  ##############################################

  def ParentDir(self, s):
    if (s.parent == None): return 'N'
    elif (s.parent.left == s): return 'L'
    else: return 'R'

  def Sibling(self, root):
    if (root.parent == None): return None
    elif (root.parent.left == root): return root.parent.right
    else: return root.parent.left

  def __FixPop(self, x):
    s = self.Sibling(x)
    if (x == self.upper): return
    elif (s == None): self.__FixPop(x.parent)
    
    elif(s.color == Color.RED):
      s.color = Color.BLACK
      s.parent.color = Color.RED
      if (self.ParentDir(x) == 'L'): self.__Rotate(s.parent, 'L')
      else: self.__Rotate(s.parent, 'R')
      self.__FixPop(x)

    elif(s.color == Color.BLACK):
      if (self.ParentDir(x) == 'L' and s.right != None and s.right.color == Color.RED):
        s.color = s.parent.color
        s.parent.color = Color.BLACK
        s.right.color = Color.BLACK
        self.__Rotate(s.parent, 'L')
        return

      elif (self.ParentDir(x) == 'R' and s.left != None and s.left.color == Color.RED):
        s.color = s.parent.color
        s.parent.color = Color.BLACK
        s.left.color = Color.BLACK
        self.__Rotate(s.parent, 'R')
        return

      elif (self.ParentDir(x) == 'L' and s.left != None and s.left.color == Color.RED):
        s.color = Color.RED
        s.left.color = Color.BLACK
        self.__Rotate(s, 'R')
        self.__FixPop(x)

      elif (self.ParentDir(x) == 'R' and s.right != None and s.right.color == Color.RED):
        s.color = Color.RED
        s.right.color = Color.BLACK
        self.__Rotate(s, 'L')
        self.__FixPop(x)
      
      else:
        s.color = Color.RED
        if (s.parent.color == Color.RED):
          s.parent.color = Color.BLACK
          return
        else: self.__FixPop(x.parent)

  def __PopProcess(self, root, num):
    if (root == None): return 
    elif (num < root.num): self.__PopProcess(root.left, num)
    elif (num > root.num): self.__PopProcess(root.right, num)
    else:
      #0 anak
      if (root.left == None and root.right == None):
        if (root == self.upper):
          self.upper = None
        elif (root.color == Color.BLACK): self.__FixPop(root)
        if (self.ParentDir(root) == 'L'): root.parent.left = None
        else: root.parent.right = None
        del root

      elif (root.left == None or root.right == None):
        rep = root.left if (root.left != None) else root.right
        if (root == self.upper):
          self.upper = rep
          self.upper.color = Color.BLACK
          self.upper.parent = None
          del root
        else:
          delC = root.color
          if (self.ParentDir(root) == 'L'): root.parent.left = rep
          else: root.parent.right = rep
          rep.parent = root.parent
          del root

          if (rep.color == Color.RED):
            rep.color = Color.BLACK
          else:
            self.__FixPop(rep)
      # 2 Anak
      else:
        subs = root.left
        while(subs.right): subs = subs.right
        root.num = subs.num
        root.data = subs.data
        self.__PopProcess(subs, subs.num)
  ##############################################

  def Push(self, num, data):
    self.upper = self.__PushProcess(self.upper, num, data)
    self.__FixPush(self.recent)

  def Pop(self, num):
    self.__PopProcess(self.upper, num)
  
  def Print(self):
    self.__PrintTree(self.upper, 0)

  def Search(self, root, num):
    if not (root): return False
    elif (num < root.num): self.Search(root.left, num)
    elif (num > root.num): self.Search(root.right, num)
    else: return True

  def InOrder(self, root):
    if (root):
      self.InOrder(root.left)
      print(root.num, end=" ")
      self.InOrder(root.right)

  def PreOrder(self, root):
    if (root):
      print(root.num, end=" ")
      self.PreOrder(root.left)
      self.PreOrder(root.right)

  def PostOrder(self, root):
    if (root):
      self.PostOrder(root.left)
      self.PostOrder(root.right)
      print(root.num, end=" ")

class AVLTree:
  def __init__(self):
    self.upper = None

  def __Rotate(self, oldRoot, code):
    parents = oldRoot.parent
    parto = 'L'
    if (parents and parents.left != oldRoot): parto = 'R'
    newRoot = None
    temp = None 
    
    if (code == 'L'):
      newRoot = oldRoot.right
      temp = newRoot.left
      newRoot.left = oldRoot
      oldRoot.right = temp
    elif (code == 'R'):
      newRoot = oldRoot.left
      temp = newRoot.right
      newRoot.right = oldRoot
      oldRoot.left = temp
    
    if (temp): temp.parent = oldRoot
    oldRoot.parent = newRoot
    newRoot.parent = parents
    if (parents): 
      if (parto == 'L'): parents.left = newRoot
      elif (parto == 'R'): parents.right = newRoot
    else: self.upper = newRoot

    self.__SetHeight(oldRoot)
    self.__SetHeight(newRoot)

    return newRoot
  
  def __SetHeight(self, root):
    if (root == None): return
    L = self.GetHeight(root.left)
    R = self.GetHeight(root.right)
    root.height = (L + 1) if (L > R) else (R + 1)

  def GetHeight(self, root):
    if (root == None): return 0
    else: return root.height

  def __BF(self, root):
    if (root == None): return 0
    L = self.GetHeight(root.left)
    R = self.GetHeight(root.right)
    return (L - R)

  def __Fix(self, root):
    self.__SetHeight(root)
    BFMv = self.__BF(root)
    BFLv = self.__BF(root.left)
    BFRv = self.__BF(root.right)

    # R
    if (BFMv >= 2 and BFLv >= 0):
      return self.__Rotate(root, 'R')
    # LR
    elif (BFMv >= 2 and BFLv < 0):
      root.left = self.__Rotate(root.left, 'L')
      return self.__Rotate(root, 'R')
    # LR
    elif (BFMv <= -2 and BFRv <= 0):
      return self.__Rotate(root, 'L')
    # RL
    elif (BFMv <= -2 and BFRv > 0):
      root.right = self.__Rotate(root.right, 'R')
      return self.__Rotate(root, 'L')
    return root

  def __PushProcess(self, root, num, data):
    if (root == None): return Node(num, data)
    elif (num < root.num):
      root.left = self.__PushProcess(root.left, num, data)
      root.left.parent = root
    elif (num > root.num):
      root.right = self.__PushProcess(root.right, num, data)
      root.right.parent = root
    return self.__Fix(root)

  def __PrintTree(self, root, s):
    if (root):
      self.__PrintTree(root.right, s+6)
      a = root.parent.num if root.parent else "OOF"
      print(f"{' '*s}{root.num}-{self.GetHeight(root)}-{a}", end="\n")
      self.__PrintTree(root.left, s+6)

  def Push(self, num, data):
    self.upper = self.__PushProcess(self.upper, num, data)
  
  def Print(self):
    self.__PrintTree(self.upper, 0)

  def Search(self, root, num):
    if not (root): return False
    elif (num < root.num): self.Search(root.left, num)
    elif (num > root.num): self.Search(root.right, num)
    else: return True

  def InOrder(self, root):
    if (root):
      self.InOrder(root.left)
      print(root.num, end=" ")
      self.InOrder(root.right)

  def PreOrder(self, root):
    if (root):
      print(root.num, end=" ")
      self.PreOrder(root.left)
      self.PreOrder(root.right)

  def PostOrder(self, root):
    if (root):
      self.PostOrder(root.left)
      self.PostOrder(root.right)
      print(root.num, end=" ")
