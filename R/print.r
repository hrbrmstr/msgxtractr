#' Pretty print a 'msg' object
#'
#' @param x a "`msg`" object
#' @param ... unused
#' @export
print.msg <- function(x, ...) {

  xto <- x$display_envelope$display_to
  if (length(xto) == 0) xto <- x$headers$To[1]
  if (length(xto) == 0) xto <- "[Unspecified]"

  xfrom <- x$headers$From[1]
  if (length(xfrom) == 0) xfrom <- "[Unspecified]"

  xsubj <- x$subject
  if (length(xsubj) == 0) xsubj <- x$headers$Subject[1]
  if (length(xsubj) == 0) xsubj <- "[Unspecified]"

  xattach <- ""
  if (length(x$attachments) > 0) xattach <- sprintf("Attachments: %s\n", length(x$attachments))

  xdate <-  ""
  if (length(x$headers$Date) > 0) xdate <- sprintf("%s\n", x$headers$Date)

  cat(sprintf("%sFrom: %s\nTo: %s\nSubject: %s\n%s", xdate, xfrom, xto, xsubj, xattach), sep="")

}