let storage = require("storage");
let path = "/ext/storage.test";

print("File exists:", storage.exists(path));

print("Writing...");
storage.write(path, "Hello ");

print("File exists:", storage.exists(path));

// Append will create the file even if it doesnt exist!
storage.append(path, "World!");

print("Reading...");
let data = storage.read(path);
print(data);

print("Removing...")
storage.remove(path);

print("Done")

// There's also:
// storage.virtualInit(path);
// storage.virtualMount();
// storage.virtualQuit();