OUT = class-files

# 1️⃣ Compile GenerateAst only
gen-compile:
	javac com/craftinginterpreters/lox/GenerateAst.java

# 2️⃣ Run GenerateAst
gen-run: gen-compile
	java com.craftinginterpreters.lox.GenerateAst com/craftinginterpreters/lox

# 3️⃣ Compile everything into class-files
build:
	rm -rf $(OUT)
	mkdir -p $(OUT)
	javac -d $(OUT) $$(find com -name "*.java")

# 4️⃣ Run Lox
run: build
	java -cp $(OUT) com.craftinginterpreters.lox.Lox

clean:
	rm -rf $(OUT)
