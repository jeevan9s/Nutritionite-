#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

const int scrollButtonPin = 18;
const int selectButtonPin = 5;

Adafruit_SSD1306 display(128, 64, &Wire, -1);

int selectedCategory = 0;
int selectedFood = 0;
int displayOffset = 0;
const int numCategories = 4;
const int numFoods = 6; // Increased number of foods
bool isDisplayingFood = false;
bool isDisplayingCategory = false;
bool isDisplayingWelcome = true;

struct Food {
  const char *name;
  float carbs;
  float protein;
  float fats;
  float calories; // Added calories
};

struct Category {
  const char *name;
  Food *foods;
};

Category categories[numCategories] = {
  {"Vegetables", new Food[numFoods]{{"Broccoli", 7.0, 2.8, 0.4, 55.0}, {"Carrot", 10.0, 0.9, 0.2, 41.0}, {"Peas", 14.0, 5.0, 0.4, 81.0}, {"Spinach", 3.6, 2.9, 0.4, 23.0}, {"Kale", 10.0, 3.3, 0.7, 49.0}, {"Cucumber", 3.6, 0.7, 0.2, 15.0}}},
  {"Meats", new Food[numFoods]{{"Chicken Breast", 0.0, 31.0, 3.6, 165.0}, {"Beef", 0.0, 26.0, 17.0, 250.0}, {"Pork", 0.0, 27.0, 13.0, 242.0}, {"Turkey", 0.0, 29.0, 7.7, 189.0}, {"Lamb", 0.0, 25.0, 20.0, 294.0}, {"Fish", 0.0, 22.0, 13.0, 206.0}}},
  {"Dairy", new Food[numFoods]{{"Milk", 12.0, 8.0, 8.0, 149.0}, {"Cheese", 1.3, 25.0, 33.0, 402.0}, {"Yogurt", 7.0, 10.0, 0.4, 59.0}, {"Butter", 0.1, 0.9, 81.1, 717.0}, {"Cream", 4.6, 2.5, 19.1, 193.0}, {"Ice Cream", 24.0, 4.6, 14.5, 207.0}}},
  {"Grains", new Food[numFoods]{{"Rice", 45.0, 4.3, 0.6, 205.0}, {"Bread", 49.0, 9.0, 1.2, 265.0}, {"Pasta", 71.0, 13.0, 1.5, 371.0}, {"Oats", 66.0, 17.0, 6.9, 607.0}, {"Corn", 74.0, 9.4, 4.7, 365.0}, {"Barley", 73.0, 12.5, 2.3, 354.0}}}
};

void displayWelcomeScreen() {
  display.clearDisplay();
  display.setTextSize(1.7);
  display.setTextColor(WHITE);
  display.setCursor(28, 20);
  display.println("NUTRITIONITE");
  display.setTextSize(1);
  display.setCursor(10, 50);
  display.println("Select to continue");
  display.display();
}

void displayCategoryList(int cursorPosition) {
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  for (int i = 0; i < numCategories && i + displayOffset < numCategories; ++i) {
    display.setCursor(0, 10 + i * 10);
    if (i + displayOffset == cursorPosition) {
      display.print(">");
    } else {
      display.print(" ");
    }
    display.println(categories[i + displayOffset].name);
  }
  display.display();
}

void displayFoodList(int cursorPosition) {
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  for (int i = 0; i < numFoods && i + displayOffset < numFoods; ++i) {
    display.setCursor(0, 10 + i * 10);
    if (i + displayOffset == cursorPosition) {
      display.print(">");
    } else {
      display.print(" ");
    }
    display.println(categories[selectedCategory].foods[i + displayOffset].name);
  }
  display.display();
}

void displayFood(int index) {
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 10);
  display.print("Food: ");
  display.println(categories[selectedCategory].foods[index].name);
  display.print("Carbs: ");
  display.println(categories[selectedCategory].foods[index].carbs);
  display.print("Protein: ");
  display.println(categories[selectedCategory].foods[index].protein);
  display.print("Fats: ");
  display.println(categories[selectedCategory].foods[index].fats);
  display.print("Calories: ");
  display.println(categories[selectedCategory].foods[index].calories);
  display.display();
}

void setup() {
  Wire.begin();
  pinMode(scrollButtonPin, INPUT_PULLUP);
  pinMode(selectButtonPin, INPUT_PULLUP);
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.clearDisplay();
  displayWelcomeScreen();
}

void loop() {
  if (digitalRead(scrollButtonPin) == HIGH) {
    if (isDisplayingCategory) {
      selectedCategory = (selectedCategory + 1) % numCategories;
      if (selectedCategory - displayOffset >= 5) {
        displayOffset++;
      } else if (selectedCategory - displayOffset < 0 && displayOffset > 0) {
        displayOffset--;
      }
      displayCategoryList(selectedCategory - displayOffset);
    } else if (isDisplayingFood) {
      selectedFood = (selectedFood + 1) % numFoods;
      if (selectedFood - displayOffset >= 5) {
        displayOffset++;
      } else if (selectedFood - displayOffset < 0 && displayOffset > 0) {
        displayOffset--;
      }
      displayFoodList(selectedFood - displayOffset);
    }
    delay(200); // Debouncing delay
  }
  if (digitalRead(selectButtonPin) == HIGH) {
    if (isDisplayingFood) {
      isDisplayingFood = false;
      displayOffset = 0;
      displayFood(selectedFood);
    } else if (isDisplayingCategory) {
      isDisplayingCategory = false;
      isDisplayingFood = true;
      displayOffset = 0;
      displayFoodList(selectedFood);
    } else {
      isDisplayingCategory = true;
      displayOffset = 0;
      displayCategoryList(selectedCategory);
    }
    delay(200); // Debouncing delay
  }
}