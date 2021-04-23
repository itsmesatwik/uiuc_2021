# Homework 4

## Satwik Singh satwiks2

### Problem 1. 
a. 
![IMG_3938](/assets/IMG_3938.jpg)
DDL:
```sql
create table Player(
    name varchar(255) not null,
    dob date not null,
    nationality varchar(255) not null,
    teamId integer,
    start_year date,
    end_year date,
    primary key (name, dob),
    foreign key (teamId) References Team(teamId) on delete cascade
)
create table Coach(
    name varchar(255) not null,
    dob date not null,
    nationality varchar(255) not null,
    teamId integer,
    primary key (name, dob),
    foreign key (teamId) References Team(teamId) on delete cascade
)
create table Team(
    teamName varchar(255) not null,
    teamId integer,
    primary key (teamId)
)
```

b. 
![IMG_3937](/assets/IMG_3937.jpg)

DDL:
```sql
create table Musician(
    stageName varchar(255) not null,
    genre varchar(255),
    primary key (stageName)
)
create table Album(
    albumNum integer not null,
    albumTitle varchar(255) not null,
    releaseDate date,
    numSongs integer,
    primary key (albumNum,albumTitle)
)
create table Song(
    songName varchar(255) not null,
    albumNum integer not null,
    albumTitle varchar(255) not null,
    duration float,
    primary key (songName, albumNum, albumTitle),
    foreign key (albumNum) references Album(albumNum) on delete cascade,
    foreign key (albumTitle) references Album(albumTitle) on delete cascade
)
create table Produces(
    stageName varchar(255) not null,
    albumNum integer not null,
    albumTitle varchar(255) not null,
    primary key (stageName, albumNum, albumTitle)
    foreign key (stageName) references Musician(stageName) on delete cascade,
    foreign key (albumNum) references Album(albumNum) on delete cascade,
    foreign key (albumTitle) references Album(albumTitle) on delete cascade
)
```
### Problem 2.

ER
![IMG_3939](/assets/IMG_3939.jpg)
UML
![Screen Shot 2021-04-01 at 11.26.38 PM](/assets/Screen%20Shot%202021-04-01%20at%2011.26.38%20PM.png)

### Problem 3. Attribute Closure and Functional Dependencies (15 pts)
Given a relation R (A, B, C, D, E) and functional dependencies FD = {C->E, DE->AC, AE->B, BC->D}, answer the following:

- Solve X+ (closure) for every subset of {A,B,C,D,E} (11 points).
- Find all candidate keys of R (2 points).
- Infer 4 non-trivial functional dependencies (2 points).

Ans: 

- A+ : A
- B+ : B
- C+ : C,E
- D+ : D
- E+ : E
- AB+ : A,B
- AC+ : A,B,C,D,E
- AD+ : A,D
- AE+ : A,B,E
- BC+ : A,B,C,D,E
- BD+ : B,D
- BE+ : B,E
- CD+ : A,B,C,D,E
- CE+ : C,E
- DE+ : A,B,C,D,E
- ABC+ : A,B,C,D,E
- ABD+ : A,B,D
- ABE+ : A,B,E
- ACD+ : A,B,C,D,E
- ACE+ : A,B,C,D,E
- ADE+ : A,B,C,D,E
- BCD+ : A,B,C,D,E
- BCE+ : A,B,C,D,E
- BDE+ : A,B,C,D,E
- CDE+ : A,B,C,D,E
- ABCD+ : A,B,C,D,E
- BCDE+ : A,B,C,D,E
- ABCDE+ : A,B,C,D,E

Candidate keys: AC, BC, CD, DE.

4 Non trivial FDs: 
- ABC -> DE
- ACD -> BE
- ACE -> BD
- ADE -> CB



### Problem 4. Normal Forms (35 pts)
1. Consider the following relations R1, R2, R3. Which normal forms (BCNF,3NF) is each relation in? Explain why the relation is or is not in each of these normal forms. (15 pts)

   - R1 = (A,B,C,D,E,F) with a set of functional dependencies FD = {E->D, DE->BC, CE->AF} (5 pts)
     - E+ : A,B,C,D,E,F
     DE+ : A,B,C,D,E,F
     CE+ : A,B,C,D,E,F
     since all the lhs in the FDs are super keys and nontrivial, R1 satisfies BCNF and 3NF both.
     <br>
   - R2=(A,B,C,D,E) with a set of functional dependencies FD={BC->AD, CD->A, AB->E, E->AC} (5pts)
     - CD+ : A,C,D
     since CD->A is nontrivial but not a super key so R2 doesn't satisfy BCNF.
     since the rhs AD is not part of the candidate keys of R2 (AB,BC,BE) so R2 is not 3NF either.
     <br>
   - R3=(A,B,C,D,E) with a set of functional dependencies FD={C->E, A->CD, B->E} (5pts)
     - C+ : CE
     since C->E is nontrivial and C is not a super key hence R3 is not BCNF.
     since E only comes in the rhs in our FDs it is impossible to have it in a candidate key thus R3 is not 3NF either. 
<br>
2. Given a relation R(A,B,C,D,E,F) with functional dependencies FD = {F->C, B->C, D->BC, CF->AE, DE->C} (10 pts)
   - Compute the minimal basis of FD (5pts)
     - RHS singletons: F->C, B->C, D->B, CF->A, CF->E, D->C, DE->C
     removing redudancies we get minimal basis: F->C, B->C, D->B, F->A, F->E
   - Decompose the relation R into a set of relations that are in 3NF.(5pts)
     - R1(ACEF) (FA)(FC)(FE) get combined, R2(BC), R3(DB), R4(DF) (added for missing key)
 <br>
3. Consider the relation R(A,B,C,D,E,F), and the corresponding set of functional dependencies FD = {B->A, D->F, BC->E, AB->D}. Decompose the relation R into a set of relations that are in BCNF. You must list all your steps to receive full points.(10 pts)
<br>


   1. B+ = {A,B,D,F}
      hence B->A is a violating FD Make R1(A,B,D,F) and R2(B,C,E).

       1. Closure in R1(A,B,D,F):
             - A+ : A
             - B+ : ABDF
             - D+ :DF
             - F+ : F
             - AB+ : ABDF
             - AD+ : ADF
             - AF+ : AF
             - BD+ : ABDF
             - BF+ : ABDF
             - DF+ : DF
             - ABD+ : ABDF
             - ABF+ : ABDF
             - ADF+ : ABDF
             - BDF+ : ABDF
             - ABDF+ : ABDF
             1. Nontrivial FDs: B->ADF, D->F, AB->DF, AD->F, BD->AF, BF->AD, ABD->F, ABF->D, BDF->A
             2. Split rule: B->A, B->D, B->F, D->F, AB->D, AB->F, AD->F, BD->A, BD->F, BF->A, BF->D, ABD->F, ABF->D, BDF->A
             3. Minimal basis after removing redundancies: B->A, B->D, D->F
             D->F is still a violating FD.
             - Split into R3(A,B,D) with FD = {B->A, B->D} and R4(D, F) with FD = {D->F} which are in BCNF
       2. Closure R2(B,C,E):
            - B+ : B
            - C+ : C
            - E+ : E
            - BC+ : BCE
            - BE+ : BE
            - CE+ : CE
            - BCE+ : BCE
            1. Nontrivial FD: BC->E
            this is also a minimal basis and in BCNF.

   BCNF form: R3(A,B,D), R4(D, F), R2(B,C,E)
