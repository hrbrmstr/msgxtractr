#' Save all attachments from a 'msg" object
#'
#' @md
#' @param msg_obj a message object read in with `read_msg()`
#' @param path directory path to save attachments in (defaults to current directory)
#' @param use_short if `TRUE` then use the "short" filename from the parsed message,
#'        otherwise use the "long" filename (if it exists)
#' @param quiet if `TRUE` then no informative messages will be displayed
#' @return a character vector of full path names of files written out (invisibly)
#' @export
#' @examples
#' x <- read_msg(system.file("extdata/unicode.msg", package="msgxtractr"))
#' td <- tempdir()
#' res <- save_attachments(x, td)
#' for (f in res) unlink(f)
save_attachments <- function(msg_obj, path=getwd(), use_short=TRUE, quiet=FALSE) {

  out <- c()

  if (!inherits(msg_obj, "msg")) stop("Can only save attachments from 'msg' objects.")

  if (length(msg_obj$attachments) > 0) {

    for (a in msg_obj$attachments) {

      if (use_short) {
        fn <- trimws(a$filename)
      } else {
        fn <- trimws(a$long_filename)
        if ((length(fn) == 0) || nchar(fn) == 0) fn <- trimws(a$filename)
      }

      out_path <- path.expand(path)
      out_path <- file.path(out_path, fn)

      if (!quiet) message(sprintf("Saving %s (%s bytes)", out_path,
                                  scales::comma(length(a$content))))

      writeBin(a$content, con = out_path)

      out <- c(out, out_path)

    }

  }

  invisible(out)

}