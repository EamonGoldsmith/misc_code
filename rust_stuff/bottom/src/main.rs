use libc::{size_t, getchar};
use std::ffi::*;

const COLOR_BLACK: c_int =	0;
const COLOR_RED: c_int = 1;
const COLOR_GREEN: c_int = 2;
const COLOR_YELLOW: c_int = 3;
const COLOR_BLUE: c_int = 4;
const COLOR_MAGENTA: c_int = 5;
const COLOR_CYAN: c_int = 6;
const COLOR_WHITE: c_int = 7;

// define the bindings to the c library, unsafe because its easier
#[link(name = "ncursesw")]
unsafe extern "C" {
    static stdscr: *mut size_t;

    fn initscr();

    fn noecho();
    fn cbreak();
    fn keypad(win: *mut size_t, bf: bool) -> c_int;

    fn curs_set(pos: c_int);

    fn has_colors() -> bool;
    fn start_color();
    fn init_pair(pair: c_int, f: c_int, b: c_int) -> c_int;
    fn attron(win: *mut size_t, pair: c_int);
    fn attroff(win: *mut size_t, pair: c_int);

    fn getmaxy(win: *mut size_t) -> c_int;
    fn getmaxx(win: *mut size_t) -> c_int;
    fn mvwprintw(win: *mut size_t, y: c_int, x: c_int, s: *const u8);

    fn refresh(win: *mut size_t);
    fn endwin() -> c_int;

    fn COLOR_PAIR(id: c_int) -> c_int;
}

fn main() {
    unsafe {
        initscr();
        noecho();
        cbreak();
        keypad(stdscr, true);
        curs_set(1);

        if !has_colors() {
            endwin();
            eprintln!("your terminal doesn't support colours");
            return;
        }

        start_color();
        init_pair(2, COLOR_BLACK, COLOR_BLUE);

        attron(stdscr, COLOR_PAIR(2));
        
        let mut x: i32 = getmaxx(stdscr);
        let mut y: i32 = getmaxy(stdscr);

        y = y / 2;
        x = (x / 2) - 6;
        
        let hello: &'static str = "hello, world";

        mvwprintw(stdscr, y, x, hello.as_bytes().as_ptr());
        mvwprintw(stdscr, y, x, hello.as_bytes().as_ptr());

        refresh(stdscr);

        attroff(stdscr, COLOR_PAIR(2));

        getchar();
        endwin();
    }
}

