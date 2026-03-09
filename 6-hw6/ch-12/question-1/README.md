### Parser.java
```
List<Stmt.Function> classMethods = new ArrayList<>();

// ...

private Stmt classDeclaration() {
  Token name = consume(IDENTIFIER, "Expect class name.");
  Expr.Variable superclass = null;
  if (match(LESS)) {
    consume(IDENTIFIER, "Expect superclass name.");
    superclass = new Expr.Variable(previous());
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

### Stmt.java
```
static class Class extends Stmt {
  Class(Token name, Expr.Variable superclass, 
        List<Stmt.Function> methods,
        List<Stmt.Function> classMethods) {
    this.name = name;
    this.superclass = superclass;
    this.methods = methods;
    this.classMethods = classMethods;
  }

  @Override
  <R> R accept(Visitor<R> visitor) {
    return visitor.visitClassStmt(this);
  }

  final Token name;
  final Expr.Variable superclass;
  final List<Stmt.Function> methods;
  final List<Stmt.Function> classMethods; // new
}
```

### LoxClass.java
```
class LoxClass extends LoxInstance implements LoxCallable {
  final String name;
  final LoxClass superclass;
  private final Map<String, LoxFunction> methods;

  private LoxClass(String name, LoxClass superclass,
                  Map<String, LoxFunction> methods) {
    super(null);
  }

  LoxClass(String name, LoxClass superclass, 
          Map<String, LoxFunction> methods,
          Map<String, LoxFunction> classMethods) {
    super(new LoxClass(name + "metaclass", null, classMethods));
    this.superclass = superclass;
    this.name = name;
    this.methods = methods;
  }

  // ...
}
```

### Interpreter.java
```
@Override
public Void visitClassStmt(Stmt.Class stmt) {
  Object superclass = null;
  if (stmt.superclass != null) {
    superclass = evaluate(stmt.superclass);
    if (!(superclass instanceof LoxClass)) {
      throw new RuntimeError(stmt.superclass.name,
          "Superclass must be a class.");
    }
  }

  environment.define(stmt.name.lexeme, null);

  if (stmt.superclass != null) {
    environment = new Environment(environment);
    environment.define("super", superclass);
  }

  Map<String, LoxFunction> methods = new HashMap<>();
  for (Stmt.Function method : stmt.methods) {
    LoxFunction function = new LoxFunction(method.name.lexeme, method.function, environment,
        method.name.lexeme.equals("init"));
    methods.put(method.name.lexeme, function);
  }

  Map<String, LoxFunction> classMethods = new HashMap<>();
  for (Stmt.Function method : stmt.classMethods) {
    LoxFunction function = new LoxFunction(method.name.lexeme, 
            method.function, environment, false);
    classMethods.put(method.name.lexeme, function);
  }

  LoxClass klass = new LoxClass(stmt.name.lexeme, 
      (LoxClass)superclass, methods, classMethods);

  if (superclass != null) {
    environment = environment.enclosing;
  }

  environment.assign(stmt.name, klass);
  return null;
}

```

### Resolver.java
```
@Override
public Void visitClassStmt(Stmt.Class stmt) {
  ClassType enclosingClass = currentClass;
  currentClass = ClassType.CLASS;

  declare(stmt.name);
  define(stmt.name);

  if (stmt.superclass != null &&
      stmt.name.lexeme.equals(stmt.superclass.name.lexeme)) {
    Lox.error(stmt.superclass.name,
        "A class can't inherit from itself.");
  }

  if (stmt.superclass != null) {
    currentClass = ClassType.SUBCLASS;
    resolve(stmt.superclass);
  }

  if (stmt.superclass != null) {
    beginScope();
    scopes.peek().put("super",
        new Variable(stmt.superclass.name, VariableState.USED));
  }

  beginScope();
  scopes.peek().put("this", true);

  for (Stmt.Function method : stmt.methods) {
    FunctionType declaration = FunctionType.METHOD;
    if (method.name.lexeme.equals("init")) {
      declaration = FunctionType.INITIALIZER;
    }

    resolveFunction(method.function, declaration);
  }

  endScope();

  for (Stmt.Function method : stmt.classMethods) {
    resolveFunction(method.function, FunctionType.METHOD);
  }

  if (stmt.superclass != null) endScope();

  currentClass = enclosingClass;
  return null;
}

```
