
mydataset <- read_csv("data-raw/MyData.csv")

usethis::use_data(mydataset, overwrite = T)