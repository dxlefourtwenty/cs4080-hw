### Interpreter.java
```
globals.define("len", new LoxCallable() {
  @Override
  public int arity() { return 1; }

  @Override
  public Object call(Interpreter interpreter,
                      List<Object> arguments) {
    Object value = arguments.get(0);
    if (!(value instanceof String)) {
      throw new RuntimeError(
          new Token(TokenType.IDENTIFIER, "len", null, 0),
          "len() expects a string.");
    }
    return (double) ((String) value).length();
  }

  @Override
  public String toString() { return "<native fn>"; }
});
```
