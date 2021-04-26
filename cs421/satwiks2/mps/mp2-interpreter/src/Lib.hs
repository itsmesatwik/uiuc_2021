module Lib where
import Data.HashMap.Strict as H (HashMap, empty, fromList, insert, lookup, union)


--- Data Types
--- ----------

--- ### Environments and Results

type Env  = H.HashMap String Val
type PEnv = H.HashMap String Stmt

type Result = (String, PEnv, Env)

--- ### Values

data Val = IntVal Int
         | BoolVal Bool
         | CloVal [String] Exp Env
         | ExnVal String
    deriving (Eq)

instance Show Val where
    show (IntVal i) = show i
    show (BoolVal i) = show i
    show (CloVal xs body env) = "<" ++ show xs   ++ ", "
                                    ++ show body ++ ", "
                                    ++ show env  ++ ">"
    show (ExnVal s) = "exn: " ++ s

--- ### Expressions

data Exp = IntExp Int
         | BoolExp Bool
         | FunExp [String] Exp
         | LetExp [(String,Exp)] Exp
         | AppExp Exp [Exp]
         | IfExp Exp Exp Exp
         | IntOpExp String Exp Exp
         | BoolOpExp String Exp Exp
         | CompOpExp String Exp Exp
         | VarExp String
    deriving (Show, Eq)

--- ### Statements

data Stmt = SetStmt String Exp
          | PrintStmt Exp
          | QuitStmt
          | IfStmt Exp Stmt Stmt
          | ProcedureStmt String [String] Stmt
          | CallStmt String [Exp]
          | SeqStmt [Stmt]
    deriving (Show, Eq)

--- Primitive Functions
--- -------------------

intOps :: H.HashMap String (Int -> Int -> Int)
intOps = H.fromList [ ("+", (+))
                    , ("-", (-))
                    , ("*", (*))
                    , ("/", (div))
                    ]

boolOps :: H.HashMap String (Bool -> Bool -> Bool)
boolOps = H.fromList [ ("and", (&&))
                     , ("or", (||))
                     ]

compOps :: H.HashMap String (Int -> Int -> Bool)
compOps = H.fromList [ ("<", (<))
                     , (">", (>))
                     , ("<=", (<=))
                     , (">=", (>=))
                     , ("/=", (/=))
                     , ("==", (==))
                     ]

--- Problems
--- ========

--- Lifting Functions
--- -----------------

liftIntOp :: (Int -> Int -> Int) -> Val -> Val -> Val
liftIntOp div _ (IntVal 0) = ExnVal "Division by 0"
liftIntOp op (IntVal x) (IntVal y) = IntVal (op x y)
liftIntOp _ _ _ = ExnVal "Cannot lift"

liftBoolOp :: (Bool -> Bool -> Bool) -> Val -> Val -> Val
liftBoolOp op (BoolVal x) (BoolVal y) = BoolVal (op x y)
liftBoolOp _ _ _ = ExnVal "Cannot lift"

liftCompOp :: (Int -> Int -> Bool) -> Val -> Val -> Val
liftCompOp op (IntVal x) (IntVal y) = BoolVal (op x y)
liftCompOp _ _ _ = ExnVal "Cannot lift"

--- Eval
--- ----

eval :: Exp -> Env -> Val

--- ### Constants

eval (IntExp i)  _ = IntVal i
eval (BoolExp i) _ = BoolVal i

--- ### Variables
--- was in lectures
eval (VarExp s) env = 
    case H.lookup s env of
        Just a -> a
        Nothing -> ExnVal "No match in env"

--- ### Arithmetic
--- was in lectures
eval (IntOpExp op e1 e2) env =
    let v1 = eval e1 env
        v2 = eval e2 env
        Just f = H.lookup op intOps
    in liftIntOp f v1 v2

--- ### Boolean and Comparison Operators
--- straightforward and in lecture
eval (BoolOpExp op e1 e2) env =
    let v1 = eval e1 env
        v2 = eval e2 env
        Just f = H.lookup op boolOps
    in liftBoolOp f v1 v2

eval (CompOpExp op e1 e2) env =
    let v1 = eval e1 env
        v2 = eval e2 env
        Just f = H.lookup op compOps
    in liftCompOp f v1 v2

--- ### If Expressions

eval (IfExp e1 e2 e3) env = 
    case (eval e1 env) of
        BoolVal True -> eval e2 env
        BoolVal False -> eval e3 env
        _ -> ExnVal "Condition is not a Bool"

--- ### Functions and Function Application
-- from interpreter repl.it
eval (FunExp params body) env = CloVal params body env

eval (AppExp e1 args) env =
    case (eval e1 env) of
        CloVal params body clenv -> let n_vals = map(\x -> eval x env) args
                                        n_env = H.fromList(zip params n_vals)
                                        final_env = H.union n_env clenv
                                    in eval body final_env
        _ -> ExnVal "Apply to non-closure"

--- ### Let Expressions
eval (LetExp pairs body) env = let n_vals = map(\(x,y) -> (x, eval y env)) pairs
                                   n_env = H.fromList(n_vals)
                                   final_env = H.union n_env env
                                in eval body final_env


--- Statements
--- ----------

-- Statement Execution
-- -------------------

exec :: Stmt -> PEnv -> Env -> Result
exec (PrintStmt e) penv env = (val, penv, env)
    where val = show $ eval e env

--- ### Set Statements

exec (SetStmt var e) penv env = ("", penv, n_env)
    where n_env = H.insert var (eval e env) env

--- ### Sequencing

exec (SeqStmt []) penv env = ("", penv, env)
exec (SeqStmt (x:xs)) penv env = (res1 ++ rec_res, penv2, env2)
    where (res1, penv1, env1) = exec x penv env
          (rec_res, penv2, env2) = exec (SeqStmt xs) penv1 env1

--- ### If Statements

exec (IfStmt e1 s1 s2) penv env =
    case eval e1 env of 
        BoolVal True -> exec s1 penv env
        BoolVal False -> exec s2 penv env
        otherwise -> (exn, penv, env)
        where exn = show $ ExnVal "Condition is not a Bool"

--- ### Procedure and Call Statements

exec p@(ProcedureStmt name args body) penv env = ("", H.insert name p penv, env)

exec (CallStmt name args) penv env =
    case H.lookup name penv of
         Just (ProcedureStmt _ params body) -> let n_vals = map(\x -> eval x env) args
                                                   n_env = H.fromList(zip params n_vals)
                                                   final_env = H.union n_env env
                                                in exec body penv final_env
         Nothing -> ("Procedure " ++ name ++ " undefined", penv, env)
