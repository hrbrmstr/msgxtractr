#' Turn a single `msg` object into a `tibble`
#'
#' @param x a `msg` object
#' @param ... passed on to [tibble::as_tibble()]
#' @export
tidy_msg <- function(x, ...) {

  x <- unclass(x)

  x <- x[lengths(x) > 0]

  for (idx in which(sapply(x, mode) == "list")) {
    x[[idx]] <- list(x[[idx]])
  }

  tibble::as_tibble(x, ...)

}

#' @rdname tidy_msg
#' @export
as_tibble.msg <- tidy_msg