instance (Ord i, Integral i) => Ord (Mod17 i) where
  Mod17 i1 <= Mod17 i2 =  i1 `mod` 17 <= i2 `mod` 17

instance (Num i, Integral i) => Num (Mod17 i) where
  Mod17 i1 + Mod17 i2  = Mod17  $ (i1 + i2) `mod` 17
  Mod17 i1 * Mod17 i2  = Mod17  $ (i1 * i2) `mod` 17
  abs (Mod17 i) = Mod17 (abs i)
  signum (Mod17 i) = Mod17 (signum i)
  negate (Mod17 i) = Mod17 (-1 * i)
  fromInteger i = Mod17 (fromInteger i)

instance Functor Mod17 where
  fmap f (Mod17 i) = Mod17 (f i)

instance Applicative Mod17 where
  pure x = Mod17 x
  Mod17 f <*> Mod17 x = Mod17 $ f x

instance Monad Mod17 where
  Mod17 x >>= f = f x