#' Read an Outlook '.msg' file into a data frame
#'
#' @param path path to '.msg' file
#' @return data frame
#' @export
#' @examples
#' read_msg(system.file("extdata/unicode.msg", package="msgxtractr"))
read_msg <- function(path) {

  path <- path.expand(path)
  if (!file.exists(path)) stop(sprintf("'%s' not found.", path), call.=FALSE)

  msg <- .msg$Message(path)

  data.frame(
    to = msg$to,
    cc = msg$cc,
    subject = msg$subject,
    date = anytime::anytime(msg$date),
    body = iconv(msg$body, from="UTF-7"),
    stringsAsFactors=FALSE
  ) -> xdf

  class(xdf) <- c("tbl_df", "tbl", "data.frame")

  xdf

}