// Lifetimes are also needed when structs hold references.

// TODO: Fix the compiler errors about the struct.
// the struct need lifetime parameters because it is holding references
// the borrow checker must know how long the struct need them
struct Book<'a> {
    author: &'a str,
    title: &'a str,
}

fn main() {
    let book = Book {
        author: "George Orwell",
        title: "1984",
    };

    println!("{} by {}", book.title, book.author);
}
