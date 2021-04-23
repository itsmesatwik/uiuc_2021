# Homework 5

## Satwik Singh satwiks2

### Problem 1.
![Screen Shot 2021-04-15 at 1.33.32 AM](https://i.imgur.com/Pvcyxqj.png)

1. 24: B0, B1, B4
2. 78: B0, B2, B7
3. [26,63]: B0, B1, B4, B5, B6
4. Insert 69, 46
   1. 69![Screen Shot 2021-04-15 at 2.21.52 AM](https://i.imgur.com/QKMLYwA.png)
   2. 46 ![Screen Shot 2021-04-15 at 2.31.55 AM](https://i.imgur.com/r8ZV8OE.png)
5. Delete 50, 9
   1. 50 ![Screen Shot 2021-04-15 at 2.37.57 AM](https://i.imgur.com/sC6XotB.png)
   2. 9 ![Screen Shot 2021-04-15 at 2.44.06 AM](https://i.imgur.com/SKP6TFq.png)

### Problem 2.
![Screen Shot 2021-04-15 at 4.36.34 PM](https://i.imgur.com/TDbE6Le.png)

1. r/N = 1.25
2. Insert 11001 ![Screen Shot 2021-04-15 at 5.04.56 PM](https://i.imgur.com/NvaTAol.png)
3. Insert 00110 
![Screen Shot 2021-04-15 at 5.06.17 PM](https://i.imgur.com/PBcFOyT.png)
Insert 00000
![Screen Shot 2021-04-15 at 5.14.11 PM](https://i.imgur.com/cOrBeVR.png)

### Problem 3.

1. 
   - SLOCK1(B); R1(B); XLOCK1(B); W1(B); SLOCK2(C); R2(C); SLOCK2(A); R2(A); XLOCK2(C); W2(C); XLOCK2(A); W2(A); REL2(A,C); XLOCK1(A); W1(A); XLOCK1(C); W1(C); REL1(A,B,C);
   - SLOCK1(A); R1(A); REL1(A); SLOCK2(C); R2(C); SLOCK2(B); R2(B); XLOCK2(A); W2(A); XLOCK1(B); DENIED1;
2. 
   - SLOCK1(B); R1(B); XLOCK1(B); W1(B); SLOCK2(C); R2(C); SLOCK2(A); R2(A); XLOCK2(C); W2(C); XLOCK2(A); W2(A); REL2(A,C); XLOCK1(A); W1(A); XLOCK1(C); W1(C); REL1(A,C);
   - SLOCK1(A); R1(A); SLOCK2(C); R2(C); SLOCK2(B); R2(B); XLOCK2(A); DENIED2;

### Problem 4.

1. 
![IMG_3949](https://i.imgur.com/Ps1X6xi.jpg)
2. S2 and S3 have cycles (T1<-->T2, T1->T4->T2->T1 respectively) so they are not conflict serializable. S1 after sorting topologically we get, T4->T2->T3->T1: R4(B); W4(B); R4(A); R2(A); R2(C); W3(A); R3(C); R3(A); W1(C); R1(C);

### Problem 5.

1. 
   - S1
   ![Screen Shot 2021-04-15 at 9.11.04 PM](https://i.imgur.com/9FyBF1P.png) 
   S1(A); R1(A); X3(C); W3(C); S3(C); S2(B); R2(B); S2(A); R2(A); S2(C); R2(C); R3(C); REL(C); X2(C); W2(C); REL(A,B,C); S1(B); R1(B); REL(A,B);
   2PL is feasible.
   - S2
   ![Screen Shot 2021-04-15 at 9.21.24 PM](https://i.imgur.com/rcmcLu3.png)
   X1(A); W1(A); S1(A); R1(A); X2(C); W2(C); S3(B); R3(B); X3(B); W3(B); X1(C); WAIT1(C);
   2PL is not feasble since T1 has to wait for T2 to release C.
<br>

2. 
   - S1 is not possible in Strict 2PL, thus reordering is required which leads to the following schedule. This is still equivalent to the original schedule.
   ![Screen Shot 2021-04-15 at 10.59.31 PM](https://i.imgur.com/xh5KkTN.png)
   - S2 is not feasible under Strict 2PL, thus reordering is required. Deadlock happens.
   ![Screen Shot 2021-04-15 at 11.03.28 PM](https://i.imgur.com/TNtEaZ5.png)