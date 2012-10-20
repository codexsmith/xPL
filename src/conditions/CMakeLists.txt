
set(conditions_source_files
    XPLCondition.cpp
    TimeCondition.cpp
    DayCondition.cpp
    GlobalCondition.cpp 
    GlobalChangedCondition.cpp
)
add_library(conditions ${conditions_source_files})
