### Interpreter.java
```
private static Object uninitialized = new Object();

Object value = uninitialized;

// modified for ch8-q2
public Object visitVariableExpr(Expr.Variable expr) {
  Object value = environment.get(expr.name);
  if (value == uninitialized) {
    throw new RuntimeError(expr.name,
        "Variable must be initialized before use.");
  }
  return value;
}
