🌡️ Weather Data Analysis & Forecasting Toolkit

This project implements a technical analysis toolkit for processing and predicting weather data using object-oriented design. It operates on hourly temperature data from various European countries (1980–2019) and provides features for candlestick computation, ASCII plotting, filtering, and trend prediction.

Data of the CSV can be obtained from https://data.open-power-system-data.org/weather_data/2020-09-16

🔧 Features

🕯️ Candlestick Data Computation
Calculates yearly Open, High, Low, Close values from hourly temperatures.

Uses median estimation for 1980 due to lack of prior data.

Groups data by year with clear encapsulation via Candlestick, TempBook, and TempBookEntry classes.

📊 Text-Based Plotting
Visualizes candlestick data using ASCII characters.

Scales to fit within 10 rows.

Allows users to select and display plots for up to 5 years.

🔍 Data Filtering
Enables filtering by year or year range.

Applies filters before generating plots for targeted insights.

📈 Prediction Module
Forecasts the next year’s candlestick data using random fluctuations from the latest year’s Close value.

Displays predicted values alongside historical trends.

Serves as a basis for future integration of advanced forecasting models.

🧠 Conclusion

The project demonstrates how OOP principles can be effectively applied to real-world data analysis. With modular components and a clear focus on usability, the toolkit offers a foundation for deeper climate insights and more sophisticated future enhancements.
