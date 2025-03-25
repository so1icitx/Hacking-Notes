# JavaScript 

## Introduction
JavaScript (JS) is a powerful programming language that runs in the browser and enables interactive web pages.

---
## Variables
### What They Are:
Variables store data values, making them reusable throughout the program.

### Ways to Declare Variables:
- `var` – Function-scoped (outdated, use with caution)
- `let` – Block-scoped (preferred for most cases)
- `const` – Block-scoped (best for values that never change)

```javascript
var globalVar = "I exist everywhere in this function";
let localVar = 25; // Only available in its block
const constantVar = "I cannot be changed";
```

### Variable Scope Example:
```javascript
if (true) {
  let localVar = 30;
  console.log(localVar); // 30 (inside block)
}
console.log(localVar); // Error! (not accessible outside the block)
```

---
## Data Types
### What They Are:
Define the type of value stored in a variable.

### Common Data Types:
- **String**: Text (`"Hello World"`)
- **Number**: Numeric values (`42`, `3.14`)
- **Boolean**: `true` or `false`
- **Null**: Intentionally empty (`null`)
- **Undefined**: Declared but not assigned
- **Object**: Complex data structures (`{name: "Alice"}`)
- **Array**: List of values (`[1, 2, 3]`)

```javascript
let username = "Alice";
let age = 20;
let isAdult = age >= 18;
let job = null;
let salary;
let person = { name: "Alice", age: 20 };
console.log(username, age, isAdult, job, salary, person);
```

---
## Functions
### What They Are:
Functions are reusable blocks of code that perform specific tasks.

### Function Syntax:
```javascript
function greet(name) {
  return `Hello, ${name}!`;
}
console.log(greet("Alice")); // "Hello, Alice!"
```

### Looping with Functions:
```javascript
function printRoll(rollNum) {
  console.log(`Student with roll ${rollNum} passed!`);
}
let rollNumbers = ["001", "002", "003"];
for (let i = 0; i < rollNumbers.length; i++) {
  printRoll(rollNumbers[i]);
}
```

---
## Loops
### What They Are:
Loops allow repetitive execution of code until a condition is met.

### Loop Types:
- `for` – Used when the number of iterations is known
- `while` – Used when the number of iterations is unknown

```javascript
let students = ["A001", "A002", "A003"];
for (let i = 0; i < students.length; i++) {
  console.log(`Result for ${students[i]}: Pass`);
}
```

```javascript
let count = 0;
while (count < 3) {
  console.log(`Count: ${count}`);
  count++;
}
```

---
## JavaScript & The Request-Response Cycle
### What It Is:
How the browser (client) requests and receives data from a server.

### Example (Basic Addition in HTML):
```html
<!DOCTYPE html>
<html>
<body>
  <p id="output"></p>
  <script>
    let x = 5, y = 10;
    document.getElementById("output").innerHTML = `Sum: ${x + y}`;
  </script>
</body>
</html>
```

---
## Integrating JavaScript with HTML
### Two Ways to Include JavaScript:
#### Internal JavaScript (Within HTML File)
Used for small scripts inside `<script>` tags.
```html
<!DOCTYPE html>
<html>
<body>
  <script>
    console.log("Internal JS Loaded");
  </script>
</body>
</html>
```

####  External JavaScript (Separate `.js` File)
Used for larger projects, improving readability and maintainability.
```html
<!DOCTYPE html>
<html>
<head>
  <script src="script.js"></script>
</head>
<body>
</body>
</html>
```
**script.js:**
```javascript
console.log("External JS Loaded");
```

---
##  User Interaction
### What It Is:
JS provides built-in dialog boxes for user interaction.

### Examples:
```javascript
alert("Welcome!");
let name = prompt("Enter your name:");
let confirmAction = confirm("Are you sure?");
console.log(name, confirmAction);
```

---
##  Control Flow
### What It Is:
Control how code executes based on conditions.

### Example (Age Check):
```javascript
let age = prompt("Enter your age:");
if (age >= 18) {
  console.log("Adult");
} else {
  console.log("Minor");
}
```

---
## Security & Best Practices
###  Secure Your JavaScript Code:
1. **Minification & Obfuscation**: Compress code to make it harder to read.
    ```javascript
    function greet() { alert("Hello"); }
    ```
    → Minified: `function g(){alert("Hello")}`
2. **Avoid Client-Side Validation Only**: Validate data on the server to prevent bypassing.
3. **No Hardcoded Secrets**: Never store sensitive data in JavaScript (`const apiKey = "1234";`).
4. **Use Trusted Libraries**: Malicious scripts can steal data.

---
##  Exploitation Insight
### How JavaScript Can Be Misused:
Attackers can inject malicious scripts to manipulate a page or annoy users.

### Example (Spam Alert Attack):
```html
<script>
  for (let i = 0; i < 500; i++) {
    alert("Hacked!");
  }
</script>
```
### Defense:
- Never open untrusted `.html` files.
- Use Content Security Policy (CSP) to block unwanted scripts.

---

 ## Obfuscation in Action 
 ### Minifying and Obfuscating Code To obfuscate JavaScript,
 visit: [JavaScript Obfuscator](https://codebeautify.org/javascript-obfuscator).
 Copy the contents of your html file, paste them into the input field, and click "Obfuscate." The tool will transform the code into unreadable characters. 
 ### Deobfuscating a Code To restore obfuscated code
 visit: [JavaScript Deobfuscator](https://obf-io.deobfuscate.io/). Paste the obfuscated code into the provided box, and the website will generate human-readable JavaScript, allowing you to analyze the original script.
  
 --- 
