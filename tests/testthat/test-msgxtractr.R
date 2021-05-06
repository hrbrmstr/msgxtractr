context("basic functionality")
test_that("we can do something", {

  `%||%` <- function (x, y) { if (is.null(x)) y else x }

  x <- read_msg(system.file("extdata/unicode.msg", package="msgxtractr"))

  expect_equal(x$subject, "Test for TIF files")
  expect_equal(x$sender$sender_name, "Brian Zhou")
  expect_equal(x$display_envelope$display_to, "brianzhou@me.com")

  # CRAN-able test for file handle disposition

  do.call(
    rbind,
    lapply(1:100, function(.x) {

      if (.x %% 10 == 0) cat(".")

      msg <- read_msg(system.file("extdata/unicode.msg", package="msgxtractr"))

      data.frame(
        date = msg$header$Date %||% NA,
        sender_name = msg$sender$sender_name %||% NA,
        display_to = msg$display_envelope$display_to %||% NA,
        subject = msg$subject %||% NA,
        body = msg$body$text %||% NA,
        stringsAsFactors = FALSE
      )

    }) -> x

  ) -> mail_df

  expect_equal(nrow(mail_df), 100)

  do.call(
    rbind.data.frame,
    lapply(1:10, function(.x) {
      tidy_msg(read_msg(system.file("extdata/unicode.msg", package="msgxtractr")))
    })
  ) -> res

  expect_equal(nrow(res), 10)

})
