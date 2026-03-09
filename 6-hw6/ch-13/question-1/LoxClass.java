package com.craftinginterpreters.lox;

import java.util.ArrayList;
import java.util.Collections;
import java.util.List;
import java.util.Map;

class LoxClass extends LoxInstance implements LoxCallable {
  final String name;
  final List<LoxClass> superclasses;
  private final Map<String, LoxFunction> methods;

  private LoxClass(String name, List<LoxClass> superclasses,
      Map<String, LoxFunction> methods) {
    super(null);
    this.superclasses = freeze(superclasses);
    this.name = name;
    this.methods = methods;
  }

  LoxClass(String name, List<LoxClass> superclasses,
      Map<String, LoxFunction> methods,
      Map<String, LoxFunction> classMethods) {
    super(new LoxClass(name + " metaclass",
        metaclassSuperclasses(superclasses), classMethods));
    this.superclasses = freeze(superclasses);
    this.name = name;
    this.methods = methods;
  }

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

  private static List<LoxClass> metaclassSuperclasses(
      List<LoxClass> superclasses) {
    if (superclasses == null || superclasses.isEmpty()) {
      return Collections.emptyList();
    }

    List<LoxClass> metaclasses = new ArrayList<>();
    for (LoxClass superclass : superclasses) {
      metaclasses.add((LoxClass) superclass.klass);
    }
    return metaclasses;
  }

  private static List<LoxClass> freeze(List<LoxClass> superclasses) {
    if (superclasses == null || superclasses.isEmpty()) {
      return Collections.emptyList();
    }

    return Collections.unmodifiableList(new ArrayList<>(superclasses));
  }

  @Override
  public String toString() {
    return name;
  }

  @Override
  public Object call(Interpreter interpreter,
                     List<Object> arguments) {
    LoxInstance instance = new LoxInstance(this);
    LoxFunction initializer = findMethod("init");
    if (initializer != null) {
      initializer.bind(instance).call(interpreter, arguments);
    }

    return instance;
  }

  @Override
  public int arity() {
    LoxFunction initializer = findMethod("init");
    if (initializer == null) return 0;
    return initializer.arity();
  }
}
