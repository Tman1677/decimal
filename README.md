# decimal
My attempt at a proper perfectly accurate decimal class for c++.

Similar to Java's BigDecimal the goal of this class is for cases where the decimal value needs to be perfectly accurate with the goal that it will throw an error before it misplaces one digit anywhere. This would make it perfect for use cases such as currency. 

This class also takes advantage of the operator overloading allowed in c++ so the data type ideally functions exactly like a primitive.
