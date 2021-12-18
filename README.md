## *Assignment 3: Animation, Texture mapping and Loading objects*


|Name|Section|B.N|
| :- | :- | :- |
|محمد مصطفي نزار|**2**|**19**|
|طارق صلاح|**1**|**44**|
|عبدالله محمد شاذلي|**1**|**52**|
|عبدالخالق عبدالملك محمد|**1**|**47**|

#### we continued our implementation of assignment 3 on implementation of task 2 as follow


##### our project is splitted into 3 scenes, every scene with one animation, different floor and can be changed through menu that pop up by right clicking on mouse :

- first scene is in snow with tree, 2 snowmen objects and running animation

- second scene is in grass with tree, ball objects and animation shotting the ball

- third scene is in desert with rock object, and jumping animation (Jumping jacks)


##### and as a bonus we added wall on all sides as texture mapping


##### issues & solutions :

- we faced a problem in stopping timer function (it's stated in documentation that it can't be stopped) so we made 3 timer functions for every animation and flags to stop the other two animations and play one only

- object's colors weren't well shown in the scene so we changed ".mtl files" for every object to show the right colors
