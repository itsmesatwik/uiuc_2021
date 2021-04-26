--- Given Code
--- ==========

module Lib where

import System.IO (hPutStrLn, hPutStr, stdout, hFlush)

import Data.List (intercalate)

import Data.Functor.Identity (Identity)
import Text.ParserCombinators.Parsec hiding (Parser)
import Text.Parsec.Prim (ParsecT)

--- The Types
--- ---------

data Stmt = Decl String [String] Exp
            deriving (Eq)

instance Show Stmt where
    show (Decl f params exp) = f ++ " " ++ intercalate " " params ++ " = " ++ (show exp)

data Exp = IntExp Integer
         | VarExp String
         | LamExp String Exp
         | IfExp Exp Exp Exp
         | OpExp String Exp Exp
         | AppExp Exp Exp
         deriving (Eq)

instance Show Exp where
    show (VarExp s)       = s
    show (IntExp i)       = show i
    show (LamExp x e)     = "(\\" ++ x ++ " -> " ++ (show e) ++ ")"
    show (IfExp e1 e2 e3) = "(if " ++ show e1 ++ " then " ++ show e2
                            ++ " else " ++ show e3 ++ ")"
    show (OpExp op e1 e2) = "(" ++ show e1 ++ " " ++ op ++ " " ++ show e2 ++ ")"
    show (AppExp f e)     = show f ++ " " ++ show e

ctorShow :: Exp -> String
ctorShow (VarExp s)       = "VarExp " ++ show s
ctorShow (IntExp i)       = "IntExp " ++ show i
ctorShow (LamExp x e)     = "LamExp " ++ show x ++ " (" ++ ctorShow e ++ ")"
ctorShow (IfExp e1 e2 e3) = "IfExp (" ++ ctorShow e1 ++ ") ("
                                ++ ctorShow e2 ++ ") ("
                                ++ ctorShow e3 ++ ")"
ctorShow (OpExp op e1 e2) = "OpExp " ++ show op ++ " ("
                                ++ ctorShow e1 ++ ") ("
                                ++ ctorShow e2 ++ ")"
ctorShow (AppExp f e)     = "AppExp (" ++ ctorShow f ++ ") (" ++ ctorShow e ++ ")"

--- Problems
--- ========

--- Manual Translation
--- ------------------

--- ### `factk :: Integer -> (Integer -> t) -> t`
-- fact n = n * fact n-1  we convert second arg into continuation
-- inspired from in class activity
factk :: Integer -> (Integer -> t) -> t
factk 0 k = k 1
factk n k = factk (n-1) (\v -> k (n*v))

--- ### `evenoddk :: [Integer] -> (Integer -> t) -> (Integer -> t) -> t`
-- evenodd (x:xs) a b = if x `mod` 2 == 0 then evenodd xs (a+x) b else evenodd xs a (b+x), 
-- we convert both cases into continuation and have a base case
evenoddk :: [Integer] -> (Integer -> t) -> (Integer -> t) -> t
evenoddk xx k1 k2 = aux xx k1 k2
    where aux [x]    k1 k2 = if x `mod` 2 == 0 then k1 x else k2 x
          aux (x:xs) k1 k2 = if x `mod` 2 == 0 then (aux xs (\v -> k1 (x+v)) k2) else (aux xs k1 (\v -> k2 (x+v)))


--- Automated Translation
--- ---------------------

gensym :: Integer -> (String, Integer)
gensym i = ("v" ++ show i, i + 1)

--- ### Define `isSimple`

isSimple :: Exp -> Bool
isSimple (AppExp _ _) = False
isSimple (IfExp e1 e2 e3) = isSimple e1 && isSimple e2 && isSimple e3
isSimple (OpExp _ e2 e3) = isSimple e2 && isSimple e3
isSimple (IntExp _) = True
isSimple (VarExp _) = True

--- ### Define `cpsExp` - Overview

cpsExp :: Exp -> Exp -> Integer -> (Exp, Integer)

--- #### Define `cpsExp` for Integer and Variable Expressions
cpsExp (IntExp i) k fresh = (AppExp k (IntExp i), fresh)
cpsExp (VarExp v) k fresh = (AppExp k (VarExp v), fresh)

--- #### Define `cpsExp` for Application Expressions
cpsExp (AppExp f e) k fresh | isSimple e        = (AppExp (AppExp f e) k, fresh)
                            | otherwise         = cpsExp e (LamExp v (AppExp (AppExp f (VarExp v)) k)) n
                                                    where (v,n) = gensym fresh
--- #### Define `cpsExp` for Operator Expressions
cpsExp (OpExp op e1 e2) k fresh 
                            | isSimple e1 && isSimple e2        = (AppExp k (OpExp op e1 e2),fresh)
                            | isSimple e1                       = let (v,n) = gensym fresh in cpsExp e2 (LamExp v (AppExp k (OpExp op e1 (VarExp v)))) n
                            | isSimple e2                       = let (v,n) = gensym fresh in cpsExp e1 (LamExp v (AppExp k (OpExp op (VarExp v) e2))) n
                            | otherwise                         = cpsExp e1 (LamExp v1 new_exp) new_fresh
                                                                    where (v1,n1) = gensym fresh
                                                                          (v2,n2) = gensym n1
                                                                          (new_exp,new_fresh) = cpsExp e2 (LamExp v2 (AppExp k (OpExp op (VarExp v1) (VarExp v2)))) n2
--- #### Define `cpsExp` for If Expressions
-- asked a friend who took this class last sem for a little help on this
cpsExp (IfExp e1 e2 e3) k fresh | isSimple e1 = let (t1,_) = cpsExp e2 k fresh
                                                    (t2,_) = cpsExp e3 k fresh 
                                                in (IfExp e1 t1 t2,fresh)
                                | otherwise     = cpsExp e1 (LamExp v (IfExp (VarExp v) t1 t2)) n
                                                    where (v,n) = gensym fresh
                                                          (t1,_) = cpsExp e2 k n
                                                          (t2,_) = cpsExp e3 k n 
--- ### Define `cpsDecl`
cpsDecl :: Stmt -> Stmt
cpsDecl (Decl f params body) = Decl f (params++["k"]) k
    where (k, _) = cpsExp body (VarExp "k") 1
