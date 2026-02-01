#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include <iomanip>

// CSV Column Indices
namespace CSVColumns {
    constexpr int SALE_ID = 0;
    constexpr int SALE_DATE = 1;
    constexpr int COUNTRY = 2;
    constexpr int REGION = 3;
    constexpr int LATITUDE = 4;
    constexpr int LONGITUDE = 5;
    constexpr int DEALERSHIP_ID = 6;
    constexpr int DEALERSHIP_NAME = 7;
    constexpr int MANUFACTURER = 8;
    constexpr int MODEL = 9;
    constexpr int VEHICLE_YEAR = 10;
    constexpr int BODY_TYPE = 11;
    constexpr int FUEL_TYPE = 12;
    constexpr int TRANSMISSION = 13;
    constexpr int DRIVETRAIN = 14;
    constexpr int COLOR = 15;
    constexpr int VIN = 16;
    constexpr int CONDITION = 17;
    constexpr int PREVIOUS_OWNERS = 18;
    constexpr int ODOMETER_KM = 19;
    constexpr int SALE_PRICE_USD = 20;
    constexpr int CURRENCY = 21;
    constexpr int FINANCING = 22;
    constexpr int PAYMENT_TYPE = 23;
    constexpr int SALES_CHANNEL = 24;
    constexpr int BUYER_ID = 25;
    constexpr int BUYER_AGE = 26;
    constexpr int BUYER_GENDER = 27;
    constexpr int BUYER_INCOME_USD = 28;
    constexpr int SALESPERSON_ID = 29;
    constexpr int SALESPERSON_NAME = 30;
    constexpr int WARRANTY_MONTHS = 31;
    constexpr int WARRANTY_PROVIDER = 32;
    constexpr int FEATURES = 33;
    constexpr int CO2_G_KM = 34;
    constexpr int MPG_CITY = 35;
    constexpr int MPG_HIGHWAY = 36;
    constexpr int ENGINE_DISPLACEMENT_L = 37;
    constexpr int HORSEPOWER = 38;
    constexpr int TORQUE_NM = 39;
    constexpr int DEALER_RATING = 40;
    constexpr int CONDITION_NOTES = 41;
    constexpr int SERVICE_HISTORY = 42;
    constexpr int MINIMUM_FIELD_COUNT = 21;
}

// Business Constants
namespace BusinessConstants {
    constexpr int TARGET_YEAR = 2025;
    constexpr int DATE_YEAR_LENGTH = 4;
    const std::string TARGET_COUNTRY = "China";
    const std::string TARGET_REGION = "Europe";
    const std::string MANUFACTURER_AUDI = "Audi";
    const std::string MANUFACTURER_BMW = "BMW";
}

class SalesRecord {
private:
    std::string sale_id;
    std::string sale_date;
    std::string country;
    std::string region;
    std::string manufacturer;
    std::string model;
    int vehicle_year;
    double sale_price_usd;

public:
    // Constructor
    SalesRecord() : vehicle_year(0), sale_price_usd(0.0) {}
    
    // Setters with validation
    void setSaleId(const std::string& id) { sale_id = id; }
    void setSaleDate(const std::string& date) { sale_date = date; }
    void setCountry(const std::string& c) { country = c; }
    void setRegion(const std::string& r) { region = r; }
    void setManufacturer(const std::string& m) { manufacturer = m; }
    void setModel(const std::string& m) { model = m; }
    void setVehicleYear(int year) { vehicle_year = year; }
    void setSalePrice(double price) { sale_price_usd = (price >= 0) ? price : 0.0; }
    
    // Getters
    const std::string& getSaleId() const { return sale_id; }
    const std::string& getSaleDate() const { return sale_date; }
    const std::string& getCountry() const { return country; }
    const std::string& getRegion() const { return region; }
    const std::string& getManufacturer() const { return manufacturer; }
    const std::string& getModel() const { return model; }
    int getVehicleYear() const { return vehicle_year; }
    double getSalePrice() const { return sale_price_usd; }
    
    // Utility methods
    int getSaleYear() const {
        if (sale_date.length() >= BusinessConstants::DATE_YEAR_LENGTH) {
            return std::stoi(sale_date.substr(0, BusinessConstants::DATE_YEAR_LENGTH));
        }
        return 0;
    }
    
    bool isEuropean() const {
        return region == BusinessConstants::TARGET_REGION;
    }
};

// Helper function to trim whitespace
std::string trim(const std::string& str) {
    size_t first = str.find_first_not_of(" \t\r\n");
    if (first == std::string::npos) return "";
    size_t last = str.find_last_not_of(" \t\r\n");
    return str.substr(first, (last - first + 1));
}

// Parse CSV line handling quoted fields
std::vector<std::string> parseCSVLine(const std::string& line) {
    std::vector<std::string> fields;
    std::string field;
    bool inQuotes = false;
    
    for (size_t i = 0; i < line.length(); ++i) {
        char c = line[i];
        
        if (c == '"') {
            inQuotes = !inQuotes;
        } else if (c == ',' && !inQuotes) {
            fields.push_back(trim(field));
            field.clear();
        } else {
            field += c;
        }
    }
    fields.push_back(trim(field));
    return fields;
}

int main() {
    std::ifstream file("world_car_sales_1m.csv");
    
    if (!file.is_open()) {
        std::cerr << "Error: Could not open world_car_sales_1m.csv" << std::endl;
        return 1;
    }
    
    std::string line;
    std::getline(file, line); // Skip header
    
    // Variables for Task 1: Audi cars sold in China in 2025
    int audiChinaCount = 0;
    
    // Variables for Task 2: BMW total revenue in 2025
    double bmwRevenue2025 = 0.0;
    
    // Variables for Task 3: Audi and BMW distribution in European countries
    std::map<std::string, int> audiEuropeDistribution;
    std::map<std::string, int> bmwEuropeDistribution;
    
    // Process each record
    while (std::getline(file, line)) {
        if (line.empty()) continue;
        
        std::vector<std::string> fields = parseCSVLine(line);
        
        // Ensure we have enough fields
        if (fields.size() < CSVColumns::MINIMUM_FIELD_COUNT) continue;
        
        SalesRecord record;
        record.setSaleId(fields[CSVColumns::SALE_ID]);
        record.setSaleDate(fields[CSVColumns::SALE_DATE]);
        record.setCountry(fields[CSVColumns::COUNTRY]);
        record.setRegion(fields[CSVColumns::REGION]);
        record.setManufacturer(fields[CSVColumns::MANUFACTURER]);
        record.setModel(fields[CSVColumns::MODEL]);
        
        try {
            record.setVehicleYear(fields[CSVColumns::VEHICLE_YEAR].empty() ? 0 : std::stoi(fields[CSVColumns::VEHICLE_YEAR]));
            record.setSalePrice(fields[CSVColumns::SALE_PRICE_USD].empty() ? 0.0 : std::stod(fields[CSVColumns::SALE_PRICE_USD]));
        } catch (const std::exception& e) {
            continue; // Skip invalid records
        }
        
        int saleYear = record.getSaleYear();
        
        // Task 1: Count Audi cars sold in China in target year
        if (record.getManufacturer() == BusinessConstants::MANUFACTURER_AUDI && 
            record.getCountry() == BusinessConstants::TARGET_COUNTRY && 
            saleYear == BusinessConstants::TARGET_YEAR) {
            audiChinaCount++;
        }
        
        // Task 2: Sum BMW revenue in target year
        if (record.getManufacturer() == BusinessConstants::MANUFACTURER_BMW && 
            saleYear == BusinessConstants::TARGET_YEAR) {
            bmwRevenue2025 += record.getSalePrice();
        }
        
        // Task 3: Count Audi and BMW in European countries
        if (record.isEuropean()) {
            if (record.getManufacturer() == BusinessConstants::MANUFACTURER_AUDI) {
                audiEuropeDistribution[record.getCountry()]++;
            } else if (record.getManufacturer() == BusinessConstants::MANUFACTURER_BMW) {
                bmwEuropeDistribution[record.getCountry()]++;
            }
        }
    }
    
    file.close();
    
    // Output Results
    std::cout << "========================================" << std::endl;
    std::cout << "CAR SALES ANALYSIS RESULTS" << std::endl;
    std::cout << "========================================" << std::endl << std::endl;
    
    // Task 1 Result
    std::cout << "TASK 1: Number of " << BusinessConstants::MANUFACTURER_AUDI 
              << " cars sold in " << BusinessConstants::TARGET_COUNTRY 
              << " in " << BusinessConstants::TARGET_YEAR << std::endl;
    std::cout << "Result: " << audiChinaCount << " cars" << std::endl << std::endl;
    
    // Task 2 Result
    std::cout << "TASK 2: Total revenue generated by BMW in " << BusinessConstants::TARGET_YEAR << std::endl;
    std::cout << "Result: $" << std::fixed << std::setprecision(2) << bmwRevenue2025 << " USD" << std::endl << std::endl;
    
    // Task 3 Result - Audi Distribution
    std::cout << "TASK 3: Distribution in European Countries (Sorted Highest to Lowest)" << std::endl;
    std::cout << "----------------------------------------" << std::endl;
    std::cout << "AUDI Distribution:" << std::endl;
    
    // Convert map to vector for sorting
    std::vector<std::pair<std::string, int>> audiSorted(
        audiEuropeDistribution.begin(), audiEuropeDistribution.end()
    );
    std::sort(audiSorted.begin(), audiSorted.end(),
              [](const std::pair<std::string, int>& a, const std::pair<std::string, int>& b) { 
                  return a.second > b.second; 
              });
    
    for (const auto& pair : audiSorted) {
        std::cout << "  " << std::setw(20) << std::left << pair.first 
                  << ": " << pair.second << " cars" << std::endl;
    }
    
    std::cout << std::endl << "BMW Distribution:" << std::endl;
    
    // Convert map to vector for sorting
    std::vector<std::pair<std::string, int>> bmwSorted(
        bmwEuropeDistribution.begin(), bmwEuropeDistribution.end()
    );
    std::sort(bmwSorted.begin(), bmwSorted.end(),
              [](const std::pair<std::string, int>& a, const std::pair<std::string, int>& b) { 
                  return a.second > b.second; 
              });
    
    for (const auto& pair : bmwSorted) {
        std::cout << "  " << std::setw(20) << std::left << pair.first 
                  << ": " << pair.second << " cars" << std::endl;
    }
    
    std::cout << std::endl << "========================================" << std::endl;
    
    return 0;
}
