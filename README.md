
![](img/card.png)

`msgxtractr` : Read Outlook '.msg' Files

A 'reticulate' wrapper to the Python 'ExtractMessage' module.

### IMPORTANT FIRST STEP

Users need Python installed and either do:

    pip install https://github.com/mattgwwalker/msg-extractor/zipball/master

or

    pip3 install https://github.com/mattgwwalker/msg-extractor/zipball/master

from the command-line depending on which version of python you have.

### TODO

-   Attachments

The following functions are implemented:

-   `read_msg` Read an Outlook '.msg' file into a data frame

### Installation

``` r
devtools::install_github("hrbrmstr/msgxtractr")
```

### Usage

``` r
library(msgxtractr)

# current verison
packageVersion("msgxtractr")
```

    ## [1] '0.1.0'

``` r
dplyr::glimpse(read_msg(system.file("extdata/unicode.msg", package="msgxtractr")))
```

    ## Observations: 1
    ## Variables: 5
    ## $ to      <chr> "brianzhou@me.com"
    ## $ cc      <chr> "Brian Zhou <brizhou@gmail.com>"
    ## $ subject <chr> "Test for TIF files"
    ## $ date    <dttm> 2013-11-18 10:26:24
    ## $ body    <chr> "This is a test email to experiment with the MS Outlook MSG Extractor\r\n\r\n\r\n-- \r\n\r\n\r\nKin...

### Test Results

``` r
library(msgxtractr)
library(testthat)

date()
```

    ## [1] "Thu Aug 24 09:08:14 2017"

``` r
test_dir("tests/")
```

    ## testthat results ========================================================================================================
    ## OK: 0 SKIPPED: 0 FAILED: 0
    ## 
    ## DONE ===================================================================================================================
