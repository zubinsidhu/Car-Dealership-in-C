/*
 * Homework 3
 * Zubin Sidhu
 * CS 240, Spring 2025
 * Purdue University
 */


#include "hw3.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


char g_rental_history[MAX_RENTALS][3][MAX_BUF_LEN] = {{{0}}};
char g_vehicle_info[MAX_RENTALS][3][MAX_BUF_LEN] = {{{0}}};
float g_rental_stats[MAX_RENTALS][4] = {{0}};
int g_rental_count = 0;

/*
 * Read Tables - Opens a file, reads in data, and populates rental history
 * vehicle info, and rental stats and returns the number of records read
 * or returns an error code
 */

int read_tables(char *file_name) {
  FILE *file = fopen(file_name, "r");
  if (file == NULL) {
    return FILE_READ_ERR;
  }

  g_rental_count = 0;

  /* Temporary variables for each field */
  char customer[MAX_BUF_LEN] = "";
  char start_date[MAX_BUF_LEN] = "";
  char end_date[MAX_BUF_LEN] = "";
  char vin[MAX_BUF_LEN] = "";
  char make[MAX_BUF_LEN] = "";
  char model[MAX_BUF_LEN] = "";
  float miles = 0;
  float rating = 0;
  float customer_fee = 0;
  float maintenance_cost = 0;

  int num = 0;
  while ((num = fscanf(file, "%49[^|]|%49[^|]|%49[^|]|%49[^,],%49[^,], \
          %49[^|]|%f|%f|%f|%f\n", customer, start_date, end_date, vin,
          make, model, &miles, &rating, &customer_fee,
          &maintenance_cost)) == 10) {

    //printf("num in loop: %d\n", num);
    //printf("rentals in loop: %d\n", g_rental_count);

    //Check maximum record count
    if (g_rental_count >= MAX_RENTALS) {
      fclose(file);
      return OUT_OF_BOUNDS;
    }

    //Check for valid date inputs
    int s_year = 0;
    int s_month = 0;
    int s_day = 0;
    int e_year = 0;
    int e_month = 0;
    int e_day = 0;

    if ((sscanf(start_date, "%d-%d-%d", &s_year, &s_month, &s_day) != 3) ||
         (sscanf(end_date, "%d-%d-%d", &e_year, &e_month, &e_day) != 3)) {
      fclose(file);
      //printf("DEBUG::Bad Date Format");
      return BAD_DATA;
    }

    if ((s_year < MIN_YEAR) || (s_year > MAX_YEAR) || (e_year < MIN_YEAR) ||
        (e_year > MAX_YEAR) || (s_month < 1) || (s_month > 12) || (s_day < 1)
        || (s_day > 30) || (e_month < 1) || (e_month > 12) || (e_day < 1) ||
        (e_day > 30) || (s_year > e_year) || ((s_year == e_year)
        && (s_month > e_month)) || ((s_year == e_year) && (s_month == e_month)
        && (s_day > e_day))) {
      fclose(file);
      //printf("DEBUG::Bad Date Values");
      return BAD_DATA;
    }

    //Check for valid numeric inputs
    if ((miles < 0) || (rating < 0) || (rating > 10) || (customer_fee <= 0) ||
       (maintenance_cost < 0)) {
      fclose(file);
      return BAD_DATA;
    }

    //Check for valid VIN
    if (strlen(vin) != 17) {
      fclose(file);
      return BAD_DATA;
    }

    for (int i = 0; i < 17; i++) {
      char c = vin[i];
      if (!(((c >= '0') && (c <= '9')) || ((c >= 'A') && (c <= 'Z')))) {
        fclose(file);
        return BAD_DATA;
      }
    }

    //Populate rental history table
    strncpy(g_rental_history[g_rental_count][0], customer, MAX_BUF_LEN - 1);
    g_rental_history[g_rental_count][0][MAX_BUF_LEN - 1] = '\0';
    strncpy(g_rental_history[g_rental_count][1], start_date, MAX_BUF_LEN - 1);
    g_rental_history[g_rental_count][1][MAX_BUF_LEN - 1] = '\0';
    strncpy(g_rental_history[g_rental_count][2], end_date, MAX_BUF_LEN - 1);
    g_rental_history[g_rental_count][2][MAX_BUF_LEN - 1] = '\0';

    //Populate vehicle info table
    strncpy(g_vehicle_info[g_rental_count][0], vin, MAX_BUF_LEN - 1);
    g_vehicle_info[g_rental_count][0][MAX_BUF_LEN - 1] = '\0';
    strncpy(g_vehicle_info[g_rental_count][1], make, MAX_BUF_LEN - 1);
    g_vehicle_info[g_rental_count][1][MAX_BUF_LEN - 1] = '\0';
    strncpy(g_vehicle_info[g_rental_count][2], model, MAX_BUF_LEN - 1);
    g_vehicle_info[g_rental_count][2][MAX_BUF_LEN - 1] = '\0';

    //Populate rental statistics table
    g_rental_stats[g_rental_count][0] = miles;
    g_rental_stats[g_rental_count][1] = rating;
    g_rental_stats[g_rental_count][2] = customer_fee;
    g_rental_stats[g_rental_count][3] = maintenance_cost;

    /*
    Debugging Input
    printf("%s,%s,%s,%s,%s,%s,%f,%f,%f,%f\n",
            g_rental_history[g_rental_count][0],
            g_rental_history[g_rental_count][1],
            g_rental_history[g_rental_count][2],
            g_vehicle_info[g_rental_count][0],
            g_vehicle_info[g_rental_count][1],
            g_vehicle_info[g_rental_count][2],
            g_rental_stats[g_rental_count][0],
            g_rental_stats[g_rental_count][1],
            g_rental_stats[g_rental_count][2],
            g_rental_stats[g_rental_count][3]);
    */

    g_rental_count++;
  }

  fclose(file);

  /*
  Debugging return value
  printf("%s,%s,%s,%s,%s,%s,%f,%f,%f,%f\n", customer, start_date, end_date,
          vin,make, model, miles, rating, customer_fee, maintenance_cost);
  printf("num: %d\n", num);
  printf("rentals: %d\n", g_rental_count);
  */

  //If assigned values wasn't 10 and it is not end of file -> Bad Data
  if (num != -1) {
    return BAD_DATA;
  }

  //No incrementation -> No Data
  if (g_rental_count == 0) {
    return NO_DATA_POINTS;
  }

  return g_rental_count <= MAX_RENTALS ? g_rental_count : OUT_OF_BOUNDS;
} /* read_tables() */

/*
 * Get Total Miles - Returns the total miles driven by the vehicle with the
 * specified VIN. If the VIN is not found, returns NOT_FOUND.
 */

float get_total_miles(char *vin) {
  if (g_rental_count == 0) {
    return NO_DATA_POINTS;
  }

  float total = 0;
  int found = 0;
  for (int i = 0; i < g_rental_count; i++) {
    if (strcmp(g_vehicle_info[i][0], vin) == 0) {
      total += g_rental_stats[i][0];
      found = 1;
    }
  }

  if (!found) {
    return NOT_FOUND;
  }

  return total;
} /* get_total_miles() */

/*
 * Avg Brand Satisfaction - Returns average customer satisfaction rating
 * for a given make. Returns NOT_FOUND if no matches.
 */

float avg_brand_satisfaction(char *make) {
  if (g_rental_count == 0) {
    return NO_DATA_POINTS;
  }

  float sum = 0;
  int count = 0;
  for (int i = 0; i < g_rental_count; i++) {
    if (strcmp(g_vehicle_info[i][1], make) == 0) {
      sum += g_rental_stats[i][1];
      count++;
    }
  }

  if (count == 0) {
    return NOT_FOUND;
  }

  //Return average satisfaction rating
  return (sum / count);
} /* avg_brand_satisfaction() */

/*
 * Average Maintenance Cost - Returns average maintenance cost of given make
 * and model. Returns NOT_FOUND if make and model not in file.
 */

float avg_maintenance_cost(char *make, char *model) {
  if (g_rental_count == 0) {
    return NO_DATA_POINTS;
  }

  float sum = 0;
  int count = 0;
  for (int i = 0; i < g_rental_count; i++) {
    if (((strcmp(g_vehicle_info[i][1], make) == 0) &&
        (strcmp(g_vehicle_info[i][2], model) == 0))) {
      sum += g_rental_stats[i][3];
      count++;
    }
  }

  if (count == 0) {
    return NOT_FOUND;
  }

  //Return average maintenance cost
  return (sum / count);
} /* avg_maintenance_cost() */

/*
 * Retire Vehicles - Populates retirable vehicles with the VIN numbers of
 * of vehicles that have accumulated a minimum of given miles across all
 * the rentals. Each VIN is added only once (in order of first appearance).
 * Returns number of vehicles added, 0 if none meet criteria, or NO_DATA_POINTS
 */

int retire_vehicles(float miles,
                    char retirable_vehicles[MAX_RENTALS][MAX_BUF_LEN]) {
  if (g_rental_count == 0) {
    return NO_DATA_POINTS;
  }

  int count = 0;
  for (int i = 0; i < g_rental_count; i++) {
    int already_processed = 0;
    for (int j = 0; j < i; j++) {
      if (strcmp(g_vehicle_info[i][0], g_vehicle_info[j][0]) == 0) {
        already_processed = 1;
        break;
      }
    }

    if (already_processed) {
      continue;
    }

    float total = 0;
    for (int k = 0; k < g_rental_count; k++) {
      if (strcmp(g_vehicle_info[k][0], g_vehicle_info[i][0]) == 0) {
        total += g_rental_stats[k][0];
      }
    }

    if (total >= miles) {
      strncpy(retirable_vehicles[count], g_vehicle_info[i][0], MAX_BUF_LEN - 1);
      retirable_vehicles[count][MAX_BUF_LEN - 1] = '\0';
      count++;
    }
  }

  return count;
} /* retire_vehicles() */

/*
 * Popular Make Model - Determines which make and model had the highest number
 * of rentals. Returns the index of the first rental where the make and model
 * was used. Returns NO_DATA_POINTS if no data exists.
 */

int popular_make_model() {
  if (g_rental_count == 0) {
    return NO_DATA_POINTS;
  }

  int popular_index = 0;
  int max_count = 0;
  for (int i = 0; i < g_rental_count; i++) {
    int count = 0;
    for (int j = 0; j < g_rental_count; j++) {
      if ((strcmp(g_vehicle_info[i][1], g_vehicle_info[j][1]) == 0) &&
          (strcmp(g_vehicle_info[i][2], g_vehicle_info[j][2]) == 0)) {
        count++;
      }
    }

    if (count > max_count) {
      max_count = count;
      popular_index = i;
    }
  }

  return popular_index;
} /* popular_make_model() */

/*
 * Calculate Vehicle Profitability - Calculate the profitability of a vehicle
 * specified by its VIN, If the result is negative, return 0. Returns NOT_FOUND
 * if the VIN is not present.
 */

float calc_vehicle_profitability(char *vin) {
  if (g_rental_count == 0) {
    return NO_DATA_POINTS;
  }

  float total_fee = 0;
  float total_maintenance = 0;
  int found = 0;
  for (int i = 0; i < g_rental_count; i++) {
    if (strcmp(g_vehicle_info[i][0], vin) == 0) {
      total_fee += g_rental_stats[i][2];
      total_maintenance += g_rental_stats[i][3];
      found = 1;
    }
  }

  if (!found) {
    return NOT_FOUND;
  }

  if (total_fee == 0) {
    return 0;
  }

  float profitability = (1 - (total_maintenance / total_fee)) * 100;
  if (profitability < 0) {
    profitability = 0;
  }

  return profitability;
} /* calc_vehicle_profitability() */

/*
 * Generate Customer Report - Generates a rental history report for the
 * customer and writes it to an output file. Report includes details for each
 * rental and an overall summary. Returns OK or an error code.
 */

int generate_customer_report(char *output_file, char *customer_name) {
  FILE *out = fopen(output_file, "w");
  if (out == NULL) {
    return FILE_WRITE_ERR;
  }

  if (g_rental_count == 0) {
    fclose(out);
    return NO_DATA_POINTS;
  }

  fprintf(out, "Rental History for %s:\n\n", customer_name);
  int cust_count = 0;
  float total_miles = 0;
  float total_fees = 0;
  int cust_indices[MAX_RENTALS] = {0};

  for (int i = 0; i < g_rental_count; i++) {
    if (strcmp(g_rental_history[i][0], customer_name) == 0) {
      cust_indices[cust_count] = i;
      cust_count++;
      fprintf(out, "Rental %d:\n", cust_count);
      fprintf(out, "%s to %s\n", g_rental_history[i][1],
              g_rental_history[i][2]);
      fprintf(out, "Vehicle: %s %s - %s\n", g_vehicle_info[i][1],
              g_vehicle_info[i][2], g_vehicle_info[i][0]);
      fprintf(out, "Miles Traveled: %.2f\n", g_rental_stats[i][0]);
      fprintf(out, "Rating: %.2f\n", g_rental_stats[i][1]);
      fprintf(out, "Customer Fee: %.2f\n\n", g_rental_stats[i][2]);
      total_miles += g_rental_stats[i][0];
      total_fees += g_rental_stats[i][2];
    }
  }

  fprintf(out, "Overall Summary:\n");
  fprintf(out, "Total Rentals: %d\n", cust_count);
  fprintf(out, "Total Miles Traveled: %.2f\n", total_miles);
  fprintf(out, "Total Customer Fees: %.2f\n", total_fees);

  if (cust_count > 0) {
    int popular_index = cust_indices[0];
    int max_occ = 0;
    for (int a = 0; a < cust_count; a++) {
      int count = 0;
      for (int b = 0; b < cust_count; b++) {
        if ((strcmp(g_vehicle_info[cust_indices[a]][1],
            g_vehicle_info[cust_indices[b]][1]) == 0) &&
            (strcmp(g_vehicle_info[cust_indices[a]][2],
            g_vehicle_info[cust_indices[b]][2]) == 0)) {
          count++;
        }
        if (count > max_occ) {
          max_occ = count;
          popular_index = cust_indices[a];
        }
        else if (count == max_occ) {
          if (cust_indices[a] < popular_index) {
            popular_index = cust_indices[a];
          }
        }
      }
    }
    fprintf(out, "Most Commonly Rented Vehicle: %s %s\n",
            g_vehicle_info[popular_index][1],
            g_vehicle_info[popular_index][2]);
  }
  else {
    fprintf(out, "Most Commonly Rented Vehicle: N/A\n");
  }
  fclose(out);
  return OK;
} /* generate_customer_report() */
