OUT = class-files

gen-compile:
	javac com/craftinginterpreters/lox/GenerateAst.java

gen-run: gen-compile
	java com.craftinginterpreters.lox.GenerateAst com/craftinginterpreters/lox

build:
	rm -rf $(OUT)
	mkdir -p $(OUT)
	javac -d $(OUT) $$(find com -name "*.java")

run: build
	java -cp $(OUT) com.craftinginterpreters.lox.Lox

clean:
	rm -rf $(OUT)
