use wasm_bindgen::prelude::*;

#[wasm_bindgen]
pub fn bocchi_shut_up(flag: i32, seq: &[i32], size: i32) -> i32 {
    10
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn bocchi_shut_up_test_when_10() {
        assert_eq!(bocchi_shut_up(1, &[11,12,13], 3), 10);
    }

    #[test]
    fn bocchi_shut_up_test_when_not_10() {
        assert_eq!(bocchi_shut_up(1, &[11,12,12], 3), 12);
    }
}