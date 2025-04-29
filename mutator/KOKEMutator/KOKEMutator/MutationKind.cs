public enum MutationKind
{
	// Compare Operators
	EqualityToInequality,         // == -> !=
	InequalityToEquality,         // != -> ==
	GreaterToLessEqual,           // >  -> <=
	LessToGreaterEqual,           // <  -> >=
	GreaterEqualToLess,           // >= -> <
	LessEqualToGreater,           // <= -> >

	// Mathematical Operators
	PlusToMinus,                  // +  -> -
	MinusToPlus,                  // -  -> +
	MultiplyToDivide,             // *  -> /
	DivideToMultiply,             // /  -> *

	// Logical Operators
	AndToOr,                      // && -> ||
	OrToAnd,                      // || -> &&

	// Bitwise Operators
	BitwiseAndToOr,               // & -> |
	BitwiseOrToAnd,               // | -> &

	// Increment/Decrement Operators
	IncrementToDecrement,         // ++ -> --
	DecrementToIncrement,         // -- -> ++

	// Genişletilebilir
	Unknown                       // Tanınmayan mutasyon
}
