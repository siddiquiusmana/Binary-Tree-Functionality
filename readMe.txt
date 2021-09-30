Binary Tree Functionality
by: Usman Siddiqui


This program shows the functionality and effectiveness of Binary Search Trees using Binary Search Trees
integrated into each node of its respective parent's Binary Search Tree. This can program can be used to
keep track of an ocean of data into their respective categories and relationships.

The program takes an input from a text file in the specified format:

********************************************************************************************************

N I Q 		//where N, I, and Q are all integers

s1 		//where {s1, s2, [...], sN} are all strings
s2 
...
sN 

s1 s12 P	//where {s1, s2, [...], sI} and {s12, s22, [...], sI2} are all strings.
s2 s22 P	//and P is any integer
...
sI sI2 P

n1		//where {n1, n2, [...], nQ} are all valid search inquiries. See INQUIRIES
n2		//for more information.
...
nQ

********************************************************************************************************

In this input: {s1, s2, [...] sN} are all names of categories and {s12, s22, [...], sI2} are all Objects
you wish to store in their respective categories. P is the count or any other value subjected to that 
particular object.

INQUIRIES:
1. "search": type "search", followed by the category and the item you're seaching for in that category. The
program will return the object and its associated value, otherwise it will notify you of the missing 
category or object
2. "item_before": type "item_before", followed by the category and the object in that particular category.
The program will return an integer specifying the number of objects in the list above the searched object.
3. "height_balance": type "height_balance", followed by a category. The program will notify you whether the
Binary Search Tree of that category is imbalanced which may adversely affect the performance of the program.
4. "count": type "count" followed by a category name. The program will return an integer specifying all the 
object's values added up from that specific category.
5. "reduce": type "reduce", followed by a category name, the object you're referring to in that category, and
the integer value you would like the object's value to be reduced by. If the object's value is reduce to less
than or equal to zero, the object is automatically deleted.
6. "delete": type "delete", followed by a category and the object of reference. The program will delete the
object from that category.
7. "delete_name": type "delete_name", followed by a category. The program will delete that category and all of
the objects within it.