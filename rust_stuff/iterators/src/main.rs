use std::collections::HashMap;

#[derive(Clone, Copy, PartialEq, Eq)]
enum Progress {
    None,
    Some,
    Complete,
}

fn count_exercise(map: &HashMap<String, Progress>, value: Progress) -> usize {
    map.values().filter(|x| **x == value).count()
}

fn count_exercise_collection(collection: &[HashMap<String, Progress>], value: Progress) -> usize {
    collection.iter().flat_map(|s| s.values()).filter(|v| **v == value).count()
}

fn get_map() -> HashMap<String, Progress> {
    use Progress::*;

    let mut map = HashMap::new();
    map.insert(String::from("variables1"), Complete);
    map.insert(String::from("functions1"), Complete);
    map.insert(String::from("hashmap1"), Complete);
    map.insert(String::from("arc1"), Some);
    map.insert(String::from("as_ref_mut"), None);
    map.insert(String::from("from_str"), None);

    map
}

fn get_vec_map() -> Vec<HashMap<String, Progress>> {
    use Progress::*;

    let map = get_map();

    let mut other = HashMap::new();
    other.insert(String::from("variables2"), Complete);
    other.insert(String::from("functions2"), Complete);
    other.insert(String::from("if1"), Complete);
    other.insert(String::from("from_into"), None);
    other.insert(String::from("try_from_into"), None);

    vec![map, other]
}

fn main() {
    use Progress::*;

    let map = get_map();
    println!("contains {} complete exercises", count_exercise(&map, Complete));
}

