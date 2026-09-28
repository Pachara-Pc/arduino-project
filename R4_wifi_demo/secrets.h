#define WIFI_SSID     "Patcher_WIFI_2.4G"
#define WIFI_PASSWORD "0825408633"
#define REFERENCE_URL "https://arduino-wifi-001-default-rtdb.asia-southeast1.firebasedatabase.app"
#define AUTH_TOKEN "KLosZ4m1nsPm4YSQ7R3nbrfHl3ncEExpRY8beADO"


double fomulaSetting ( int DAP ,  char setting , int numberConfig) {


    if(setting == 'A'){

      switch(numberConfig){
        case 1 : return (0.0846 + (0.0576 * DAP)) + ((-0.0005) * (DAP * DAP));  //Maize
        case 2 : return ((-1.25*(0.0000001)) * (DAP * DAP * DAP)) + ((5.79 * (0.00001))* (DAP * DAP)) + (-0.00495 * DAP) + 0.298;  //Cassava
        case 3 : return (-0.0004 * (DAP * DAP)) + (0.0498 * DAP) - 0.2429;  //Peanut
        case 4 : return (-0.0000002* (DAP * DAP * DAP)) + (0.00008* (DAP * DAP)) + (-0.0051* DAP) + 0.3722;  //Cassava RY15 75
        case 5 : return (-0.0000001 * (DAP * DAP * DAP)) + (0.00003 * (DAP * DAP)) + (0.0032 * DAP); //Cassava RY15 65
        case 6 : return (-0.000000111970124 * (DAP * DAP * DAP)) + (0.000048017800101 * (DAP * DAP)) - (0.001865068133119 * DAP) + 0.337026541160806; // Sugarcane PT
        case 7 : return (-0.0004725 * (DAP * DAP)) + (0.04755 * DAP) + 0.41577;  //Soybean
        default: return 1;
      }
    }else if (setting == 'B'){
        switch(numberConfig){
        case 1 : return (0.0846 + (0.0576 * DAP)) + ((-0.0005) * (DAP * DAP));  //Maize
        case 2 : return ((-1.25*(0.0000001)) * (DAP * DAP * DAP)) + ((5.79 * (0.00001))* (DAP * DAP)) + (-0.00495 * DAP) + 0.298;  //Cassava
        case 3 : return (-0.0004 * (DAP * DAP)) + (0.0498 * DAP) - 0.2429;  //Peanut
        case 4 : return (-0.0000002* (DAP * DAP * DAP)) + (0.00008* (DAP * DAP)) + (-0.0051* DAP) + 0.3722;  //Cassava RY15 75
        case 5 : return (-0.0000001 * (DAP * DAP * DAP)) + (0.00003 * (DAP * DAP)) + (0.0032 * DAP); //Cassava RY15 65
        case 6 : return (-0.000000111970124 * (DAP * DAP * DAP)) + (0.000048017800101 * (DAP * DAP)) - (0.001865068133119 * DAP) + 0.337026541160806; // Sugarcane PT
        case 7 : return (-0.0004725 * (DAP * DAP)) + (0.04755 * DAP) + 0.41577;  //Soybean
        default: return 1;
      }

    }
  

    return 1;
}

// Function to check if the given year is a leap year or not
int isLeapYear(int year) {
  if (year % 400 == 0)
    return 1;
  if (year % 100 == 0)
    return 0;
  if (year % 4 == 0)
    return 1;
  return 0;
}

// Function to calculate the number of days in a given month
int getDaysInMonth(int month, int year) {
  int days[] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
  if (month == 2 && isLeapYear(year))
    return 29;
  return days[month - 1];
}

int findDifferentDay(int day1, int month1, int year1, int day2, int month2, int year2) {
  // Calculate the total number of days for date 1
  int totalDays1 = day1;
  for (int i = 1; i < month1; i++)
    totalDays1 += getDaysInMonth(i, year1);

  // Calculate the total number of days for date 2
  int totalDays2 = day2;
  for (int i = 1; i < month2; i++)
    totalDays2 += getDaysInMonth(i, year2);

  // Calculate the total number of days for the years between the two dates
  int totalDaysBetweenYears = 0;
  for (int i = year1 ; i < year2; i++)
    totalDaysBetweenYears += isLeapYear(i) ? 366 : 365;

  // Calculate the final difference in days
  return totalDaysBetweenYears + (totalDays2 - totalDays1);
}


void workLib(){
  Serial.println("it work");
  }
