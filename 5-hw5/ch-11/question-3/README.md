### Resolver.java
```
private final Stack<Map<String, Variable>> scopes = new Stack<>();

//...

private enum VariableState {
  DECLARED,
  DEFINED,
  USED
}

private static class Variable {
  final Token name;
  VariableState state;

  Variable(Token name, VariableState state) {
    this.name = name;
    this.state = state;
  }
}

//...
private void beginScope() {
  scopes.push(new HashMap<String, Variable>());
}

private void endScope() {
  Map<String, Variable> scope = scopes.peek();

  for (Map.Entry<String, Variable> entry : scope.entrySet()) {
    if (entry.getValue().state != VariableState.USED) {
      Lox.error(entry.getValue().name, "Local variable is never initialized.");
    }
  }

  scopes.pop();
}

//...

private void resolveLocal(Expr expr, Token name) {
for (int i = scopes.size() - 1; i >= 0; i--) {
    if (scopes.get(i).containsKey(name.lexeme)) {
      scopes.get(i).get(name.lexeme).state = VariableState;
      interpreter.resolve(expr, scopes.size() - 1 - i);
      return;
    }
  }
}


```
