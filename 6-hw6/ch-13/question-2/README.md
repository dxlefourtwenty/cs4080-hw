### TokenType.java
```
// Keywords.
  AND, BREAK, CLASS, ELSE, FALSE, FUN, FOR, IF, NIL, OR,
  PRINT, RETURN, SUPER, INNER, THIS, TRUE, VAR, WHILE,
```

### Scanner.java
```
static {
  keywords = new HashMap<>();
  keywords.put("and",    AND);
  keywords.put("class",  CLASS);
  keywords.put("else",   ELSE);
  keywords.put("false",  FALSE);
  keywords.put("for",    FOR);
  keywords.put("fun",    FUN);
  keywords.put("if",     IF);
  keywords.put("nil",    NIL);
  keywords.put("or",     OR);
  keywords.put("print",  PRINT);
  keywords.put("return", RETURN);
  keywords.put("super",  SUPER);
  keywords.put("this",   THIS);
  keywords.put("true",   TRUE);
  keywords.put("var",    VAR);
  keywords.put("while",  WHILE);
  keywords.put("inner", INNER);
  keywords.put("break", BREAK);
}
```

### GenerateAst.java
```
defineAst(outputDir, "Expr", Arrays.asList(
  "Assign      : Token name, Expr value", 
  "Binary      : Expr left, Token operator, Expr right",
  "Grouping    : Expr expression",
  "Literal     : Object value",
  "Logical     : Expr left, Token operator, Expr right",
  "Set         : Expr object, Token name, Expr value",
  "Super       : Token keyword, Token method",
  "This        : Token keyword",
  "Inner       : Token keyword",
  "Unary       : Token operator, Expr right",
  "Conditional : Expr condition, Expr thenBranch, Expr elseBranch",
  "Function    : List<Token> parameters, List<Stmt> body",
  "Call        : Expr callee, Token paren, List<Expr> arguments",
  "Get         : Expr object, Token name",
  "Variable    : Token name" 
));
```

### Parser.java
```
if (match(INNER)) return new Expr.Inner(previous()); 
```

### Interpreter.java
```
private final java.util.Deque<LoxCallable> innerStack = new java.util.ArrayDeque<>();
// ...

@Override
public Object visitInnerExpr(Expr.Inner expr) {
  if (innerStack.isEmpty()) {
    // inner() does nothing if there is no matching subclass method.
    return new LoxCallable() {
      @Override public int arity() { return 0; }
      @Override public Object call(Interpreter interpreter,
            java.util.List<Object> arguments){
        return null;
      }
      @Override public String toString() { return "<inner-noop>"; }
    };
  }
  return innerStack.peek();
}
```

### LoxClass.java
```
java.util.List<LoxFunction> findMethodChain(String name) {
  java.util.ArrayList<LoxFunction> chain = new java.util.ArrayList<>();

  // First collect superclass implementations (top of the chain).
  for (LoxClass superclass : superclasses) {
    chain.addAll(superclass.findMethodChain(name));
  }

  // Then add this class's implementation (lower in the chain)
  LoxFunction here = methods.get(name);
  if (here != null) chain.add(here);

  return chain; // top -> bottom
}
```

### LoxMethodChain.java
```
package com.craftinginterpreters.lox;

import java.util.List;

class LoxMethodChain implements LoxCallable {
  private final List<LoxFunction> chain;
  private final LoxInstance receiver;
  private final int index;

  LoxMethodChain(List<LoxFunction> chain, LoxInstance receiver, int index) {
    this.chain = chain;
    this.receiver = receiver;
    this.index = index;
  }

  @Override
  public int arity() {
    return chain.get(index).arity();
  }

  @Override
  public Object call(Interpreter interpreter, List<Object> arguments) {
    LoxCallable nextInner;
    if (index + 1 < chain.size()) {
      nextInner = new LoxMethodChain(chain, receiver, index + 1);
    } else {
      nextInner = new LoxCallable() {
        @Override public int arity() { return 0; }
        @Override public Object call(Interpreter i, List<Object> args) { return null; }
        @Override public String toString() { return "<inner-noop>"; }
      };
    }

    interpreter.pushInner(nextInner);
    try {
      return chain.get(index).bind(receiver).call(interpreter, arguments);
    } finally {
      interpreter.popInner();
    }
  }

  @Override
  public String toString() {
    return "<method-chain " + index + ">";
  }
}
```

### LoxInstance.java
```
Object get(Token name) {
  if (fields.containsKey(name.lexeme)) {
    return fields.get(name.lexeme);
  }

  java.util.List<LoxFunction> chain = klass.findMethodChain(name.lexeme);
  if (!chain.isEmpty()) {
    return new LoxMethodChain(chain, this, 0);
  }

  throw new RuntimeError(name, 
      "Undefined property '" + name.lexeme + "'.");
}
```

### Resolver.java
```
@Override
public void visitInnerExpr(Expr.Inner expr) {
  return null;
}
```

### AstPrinter.java
```
@Override
public String visitInnerExpr(Expr.Inner expr) {
  return parenthesize("inner");
}
```
