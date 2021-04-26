--- Getting Started
--- ===============

--- Relevant Files
--- --------------

module Lib where

-- This line imports the Prelude module without certain functions
import Prelude hiding ( take, drop, reverse
                      , zip, zipWith
                      , map, foldl, foldr
                      , iterate, repeat
                      , replicate, cycle
                      , (++)
                      )
-- When you are allowed to use builtin functions Prepend them with "P."
-- for example `P.take`
import qualified Prelude as P

--- Problems
--- ========

--- Recursion
--- ---------

--- ### mytake
mytake :: Int -> [a] -> [a]
mytake a [] = []
mytake a (x:xs) | a <=0     = []
                | otherwise = x:(mytake (a-1) xs)
-- don't forget to put the type declaration or you will lose points!

--- ### mydrop
mydrop :: Int -> [a] -> [a]
mydrop a [] = []
mydrop a (x:xs) | a <= 0    = x:xs
                | otherwise = mydrop (a-1) xs
-- don't forget to put the type declaration or you will lose points!

-- looked up 'ways to reverse a list in haskell'
-- and found a method on http://www.runtianz.com/2015/10/02/ways-to-reverse-a-list-in-haskell/
--- ### rev
rev :: [a] -> [a]
rev [] = []
rev a = rev' a []
             where rev' [] b = b
                   rev' (x:xs) b = rev' xs (x:b)
-- don't forget to put the type declaration or you will lose points!

--- ### app
app :: [a] -> [a] -> [a]
app [] a = a
app a [] = a
app (x:xs) y = x:(app xs y)
-- don't forget to put the type declaration or you will lose points!


--- ### inclist
inclist :: Num a => [a] -> [a] 
inclist [] = []
inclist (a:ax) = a+1 : inclist ax
-- don't forget to put the type declaration or you will lose points!


--- ### sumlist
sumlist :: Num a => [a] -> a
sumlist [] = 0
sumlist (x:xs) = x + sumlist xs
-- don't forget to put the type declaration or you will lose points!


--- ### myzip
myzip :: [a] -> [b] -> [(a,b)]
myzip a [] = []
myzip [] a = []
myzip (x:xs) (y:ys) = (x,y) : (myzip xs ys)
-- don't forget to put the type declaration or you will lose points!


--- ### addpairs
addpairs :: (Num a) => [a] -> [a] -> [a]
addpairs a [] = []
addpairs [] a = []
addpairs (x:xs) (y:ys) = (x+y) : (addpairs xs ys)
-- don't forget to put the type declaration or you will lose points!

--- ### ones
ones :: [Integer]
ones = [1,1..]
-- don't forget to put the type declaration or you will lose points!


--- ### nats
nats :: [Integer]
nats = [0..]
-- don't forget to put the type declaration or you will lose points!


--- ### fib
fib :: [Integer]
fib = 0 : 1 : (addpairs fib (tail fib))
-- don't forget to put the type declaration or you will lose points!


--- Set Theory
--- ----------

--- ### add
add :: Ord a => a -> [a] -> [a]
add a [] = [a]
add a (x:xs) | a == x       = x : xs
             | a < x        = a : x : xs
             | otherwise    = x : (add a xs)
-- don't forget to put the type declaration or you will lose points!


--- ### union
union :: Ord a => [a] -> [a] -> [a]
union a [] = a
union [] a = a
union (x:xs) (y:ys) | x == y            = x : (union xs ys)
                    | x > y             = y : (union (x:xs) ys)
                    | otherwise         = x : (union xs (y:ys))
-- don't forget to put the type declaration or you will lose points!


--- ### intersect
intersect :: Ord a => [a] -> [a] -> [a]
intersect [] a = []
intersect a [] = []
intersect (x:xs) (y:ys) | x == y    = x : (intersect xs ys)
                        | x < y     = intersect xs (y:ys)
                        | otherwise = intersect (x:xs) ys
-- don't forget to put the type declaration or you will lose points!


--- ### powerset
powerset :: Ord a => [a] -> [[a]]
powerset [] = [[]]
powerset (x:xs) = union (powerset xs) (P.map(x:) (powerset xs)) 
-- don't forget to put the type declaration or you will lose points!


--- Higher Order Functions
--- ----------------------

--- ### inclist'
inclist' :: Num a => [a] -> [a]
inclist' a = P.map (+1) a
-- don't forget to put the type declaration or you will lose points!


--- ### sumlist'
sumlist' :: (Num a) => [a] -> a
sumlist' a = P.foldr (+) 0 a
-- don't forget to put the type declaration or you will lose points!

