
struct User {
    email: String,
    username: String,
    password: String,
}

fn main() {
    let user = User {
        email: "person@domain.com".to_string(),
        username: "fastSalad".to_string(),
        password: "1234".to_string(),
    };
    
    // access member of struct using pattern match
    let User{ email: a, .. } = user;
    let User{ username: b, .. } = user;
    let User{ password: c, .. } = user;

    println!("{}", a);
    println!("{}", b);
    println!("{}", c);
}
