### LoxClass.java
```
final List<LoxClass> superclassess;

private LoxClass(String name, List<LoxClass> superclasses,
                  Map<String, LoxFunction> methods) {
  super(null);
  this.superclasses = superclasses;
  this.name = name;
  this.methods = methods;
}

//...

LoxFunction findMethod(String name) {
  if (methods.containsKey(name)) {
    return methods.get(name);
  }

  for (LoxClass superclass : superclasses) {
    LoxFunction method = superclass.findMethod(name);
    if (method != null) return method;
  }

  return null;
}
```

### Parser.java
```
private Stmt classDeclaration() {
  Token name = consume(IDENTIFIER, "Expect class name.");
  Expr.Variable superclass = null;

  List<Expr.Variable> superclasses = new ArrayList<>();
  if (match(LESS)) {
    do {
      consume(IDENTIFIER, "Expect superclass name.");
      superclasses.add(new Expr.Variable(previous()));
    } while (match(COMMA));
  }

  consume(LEFT_BRACE, "Expect '{' before class body.");

  List<Stmt.Function> methods = new ArrayList<>();
  while (!check(RIGHT_BRACE) && !isAtEnd()) {
    if (match(CLASS)) {
      classMethods.add(function("method"));
    } else {
      methods.add(function("method"));
    }
  }

  consume(RIGHT_BRACE, "Expect '}' after class body.");

  return new Stmt.Class(name, superclass, methods, classMethods);
}
```

### Resolver.java
```
@Override
public Void visitClassStmt(Stmt.Class stmt) {
  for (Expr.Variable superclass : stmt.superclasses) {
    if (stmt.name.lexeme.equals(superclass.name.lexeme)) {
      Lox.error(superclass.name,
          "A class can't inherit from itself.");
    }
  }

  if (!stmt.superclasses.isEmpty()) {
    currentClass = ClassType.SUBCLASS;
    for (Expr.Variable superclass : stmt.superclasses) {
      resolve(superclass);
    }
  }

  if (!stmt.superclasses.isEmpty()) {
    beginScope();
    scopes.peek().put("super", 
        new Variable(stmt.superclass.name, VariableState.USED));
  }

  beginScope();
  scopes.peek().put("this", 
      new Variable(stmt.name, VariableState.USED));

  for (Stmt.Function method : stmt.methods) {
    FunctonType declaration = FunctionType.METHOD;
    if (method.name.lexeme.equals("init")) {
      declaration = FunctionType.INITIALIZER;
    }

    resolveFunction(method, declaration);
  }

  endScope();
  
  if (!stmt.superclasses.isEmpty()) endScope();

  for (Stmt.Function method : stmt.classMethods) {
    resolveFunction(method.function, FunctionType.METHOD);
  }

  if (stmt.superclass != null) endScope();

  currentClass = enclosingClass;
  return null;
}
```

### Stmt.java
```
static class Class extends Stmt {
  Class(Token name, List<Expr.Variable> superclasses, 
        List<Stmt.Function> methods,
        List<Stmt.Function> classMethods) {
    this.name = name;
    this.superclasses = superclasses;
    this.methods = methods;
    this.classMethods = classMethods;
  }

  @Override
  <R> R accept(Visitor<R> visitor) {
    return visitor.visitClassStmt(this);
  }

  final Token name;
  final List<Expr.Variable> superclasses;
  final List<Stmt.Function> methods;
  final List<Stmt.Function> classMethods;
}
```

### ASTPrinter.java
```
@Override
public String visitBreakStmt(Stmt.Break stmt) {
  return "break";
}

@Override
public String visitClassStmt(Stmt.Class stmt) {
  StringBuilder builder = new StringBuilder();
  builder.append("(class " + stmt.name.lexeme);

  if (!stmt.superclasses.isEmpty()) {
    builder.append(" <");
    for (Expr.Variable superclass : stmt.superclasses) {
      builder.append(" " + print(superclass));
    }
  }
}
```
