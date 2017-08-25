context("basic functionality")
test_that("we can do something", {

  x <- read_msg(system.file("extdata/unicode.msg", package="msgxtractr"))

  expect_equal(x$subject, "Test for TIF files")
  expect_equal(x$sender$sender_name, "Brian Zhou")
  expect_equal(x$display_envelope$display_to, "brianzhou@me.com")

})
