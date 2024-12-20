use std::ffi::c_char;

#[repr(C)]
pub struct Field{
    pub length: u64,
    pub name: *const c_char,
    pub endian: *const c_char
}
extern "C" {
    fn create_field() -> Field;
}
pub fn rcreate_field() -> Field {
    unsafe { create_field() }
}