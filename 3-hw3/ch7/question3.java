// case SLASH:
//   checkNumberOperands(expr.operator, left, right);
//   return (double) left / (double) right;
//
//   go from this ^
//   to this 

case SLASH:
  checkNumberOperands(expr.operator, left, right);

  double divisor = (double) right;
  if (divisor == 0.0) {
    throw new RuntimeError(expr.operator, "Division by zero.");
  }

  return (double) left / divisor;
