--- Getting Started
--- ===============

--- Relevant Files
--- --------------

module Lib where


--- Problems
--- ========

--- Algebraic Data Types
--- --------------------

data List a = Cons a (List a)
            | Nil
  deriving (Show, Eq)

data Exp = IntExp Integer
         | PlusExp [Exp]
         | MultExp [Exp]
  deriving (Show, Eq)

--- ### list2cons
list2cons :: [a] -> List a
list2cons [] = Nil
list2cons (x:xs) = Cons x (list2cons xs)
-- don't forget to put the type declaration or you will lose points!


--- ### cons2list
cons2list :: List a -> [a]
cons2list Nil = []
cons2list (Cons i l) = i : (cons2list l)
-- don't forget to put the type declaration or you will lose points!


--- ### eval
eval :: Exp -> Integer
eval (IntExp a) = a
eval (PlusExp a) = foldr (+) 0 (map eval a)
eval (MultExp a) = foldr (*) 1 (map eval a)
-- don't forget to put the type declaration or you will lose points!


--- ### list2cons'
list2cons' :: [a] -> List a
list2cons' a = foldr Cons Nil a
-- looked at definition of foldr in the slides and used it from there 
--f x0(f x1···(f xn z)···) along with the idea that cons is a function
-- don't forget to put the type declaration or you will lose points!


--- ### BinTree
data BinTree a = Node a (BinTree a) (BinTree a)
               | Leaf
  deriving (Show)
-- BinTree

--- ### sumTree
sumTree :: Num a => BinTree a -> a
sumTree Leaf = 0
sumTree (Node a l r) = a + (sumTree l) + (sumTree r)
-- don't forget to put the type declaration or you will lose points!


--- ### SimpVal
data SimpVal = IntVal Integer
               | BoolVal Bool
               | StrVal String
               | ExnVal String
  deriving (Show)
-- SimpVal

--- ### liftIntOp
liftIntOp :: (Integer -> Integer -> Integer) -> SimpVal -> SimpVal -> SimpVal
liftIntOp op (IntVal l) (IntVal r) = IntVal (l `op` r) -- notation from lecture
liftIntOp _ _ _ = ExnVal "not an IntVal!"
-- don't forget to put the type declaration or you will lose points!

