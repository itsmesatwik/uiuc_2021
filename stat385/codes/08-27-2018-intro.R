# Please register at: https://rstudio.cloud/spaces/3161/join?access_code=X9QejwISssn6WeI%2FaeTkKiAkwiB8ykuWCEFSxPgu

# Setup RStudio Cloud: https://github.com/stat385uiuc/rstudio-cloud-intro/raw/master/01-rstudio-cloud-stat385.pdf

# Install packages or extensions to R language found on CRAN
# Uncomment to run the command by deleting the `#` before it.

# install.packages("ggplot2")

# Enable the package by loading
library("ggplot2")

# Create a scatter plot
ggplot(mpg, aes(class, hwy)) + 
  geom_point()

# Plot a jittered scatter plot
ggplot(mpg, aes(class, hwy)) + 
  geom_point() + geom_jitter()

# Create a box plot
ggplot(mpg, aes(class, hwy)) + 
  geom_boxplot()

# Combine both!
ggplot(mpg, aes(class, hwy)) + 
  geom_point() + geom_boxplot() 

# Another popular way of a similar visualization
ggplot(mpg, aes(class, hwy)) + 
  geom_violin()

# Split data based the type of class it is.
ggplot(mpg, aes(displ, hwy)) +
  geom_point() +
  facet_wrap(~class)


######### More advanced

# install.packages(c("tidyverse", "raster", "leaflet"))
library("tidyverse")

# Embed file contents
my_table = read.table(text = "County     fips         Accidents
                      CAYUGA              36011    1
                      ERIE               36029    6
                      GENESEE     36037    993
                      LIVINGSTON              36051    474
                      MADISON   36053    1
                      MONROE    36055    12337
                      WAYNE        36117    1142
                      ONTARIO    36069    1671
                      ORLEANS     36073    351
                      WYOMING 36121    405
                      SENECA        36099    454
                      YATES           36123    158
                      NIAGARA    36063    1", header = TRUE)

# Fix county names 

# Force all strings to lowercase
my_table$County = tolower(my_table$County)

# Capitalize first letter
substr(my_table$County, 1, 1) = toupper(substr(my_table$County, 1, 1))

library("raster")

# Download the data once
# usa = getData("GADM", country = "USA", level = 2)

# Read in the R Binary version of the USA Shapefile
usa = readRDS("GADM_2.8_USA_adm2.rds")

# Grab only counties in New York
ny = subset(usa, NAME_1 == "New York")

# Merge data with accident information
counted_ny = merge(ny, my_table, by.x = "NAME_2", by.y = "County", all.x = TRUE)

# Create a summary popup
county_popup = paste0("<strong>County Name: </strong>",
                      counted_ny$NAME_2, "<br> <strong>",
                      ifelse(!is.na(counted_ny$Accidents),
                             paste0("Number of Accidents is: </strong> ", counted_ny$Accidents),
                             paste0("No accident data is available. </strong>")))

# Let's make it interactive!
library("leaflet")
leaflet() %>%
  setView(lng = -76.5,  # Set the level of zoom
          lat = 42.7128,
          zoom = 6) %>%
  addTiles() %>%        # Add map background (can be removed if only polygons desired)
  addPolygons(data=counted_ny,
              layerId=~NAME_2,
              fillColor=~colorQuantile("YlOrRd", Accidents)(Accidents),
              fillOpacity = 0.7,
              color = "#BDBDC3",
              weight = 1,
              popup = county_popup)
