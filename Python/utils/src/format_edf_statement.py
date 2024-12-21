import pandas as pd

def trim_csv(input_file, output_file):
    """
    This function reads a CSV file, removes unnecessary rows and columns, and writes the result to a new CSV file.

    Parameters:
    input_file (str): The path to the input CSV file.
    output_file (str): The path to the output CSV file.
    """
    # Read the CSV file
    df = pd.read_csv(input_file, sep=';', skiprows=3, encoding='ISO-8859-1')

    # Remove all unnecessary line breaks
    df = df.dropna(how='all')

    # Remove the 'Nature de la donnée' column
    df = df.drop('Nature de la donnée', axis=1)

    # Save the dataframe to a new CSV file
    df.to_csv(output_file, sep=';', encoding='ISO-8859-1', float_format='%.0f', index=False)

def remove_duplicates(input_file, output_file):
    """
    This function removes consecutive duplicate rows from a CSV file using pandas.

    Parameters:
    input_file (str): The path to the input CSV file.
    output_file (str): The path to the output CSV file.
    """
    # Read the CSV file
    df = pd.read_csv(input_file, sep=';', encoding='ISO-8859-1')

    # Remove consecutive duplicates
    df = df.loc[(df.shift() != df).any(axis=1)]

    # Save the dataframe to a new CSV file
    df.to_csv(output_file, sep=';', encoding='ISO-8859-1', float_format='%.0f', index=False)

def format_data(input_file, output_file):
    """
    This function reads a CSV file, formats the date and time, and writes the result to a new CSV file.

    Parameters:
    input_file (str): The path to the input CSV file.
    output_file (str): The path to the output CSV file.
    """
    # Read the CSV file
    df = pd.read_csv(input_file, sep=';', encoding='ISO-8859-1')

    # Remove rows where 'Date et heure de relève par le distributeur' is NaN
    df = df.dropna(subset=['Date et heure de relève par le distributeur'])
    
    # Split dates and times into two separate columns
    df['Date'] = df['Date et heure de relève par le distributeur'].apply(lambda x: x if ':' not in x else None)
    df['Time'] = df['Date et heure de relève par le distributeur'].apply(lambda x: x if ':' in x else None)

    # Convert dates to datetime and times to timedelta
    df['Date'] = pd.to_datetime(df['Date'], format='%d/%m/%Y', errors='coerce')
    df['Time'] = pd.to_timedelta(df['Time'])

    # Propagate the last valid date to each time
    df['Date'] = df['Date'].ffill()

    # Create a new Datetime column by adding the date and time
    df['Date et heure de relève par le distributeur'] = df['Date'] + df['Time']
    df = df.drop(columns=['Date', 'Time'])

    # Remove rows where 'Puissance atteinte (W)' is NaN
    df = df.dropna(subset=['Puissance atteinte (W)'])

    # Save the dataframe to a new CSV file
    df.to_csv(output_file, sep=';', encoding='ISO-8859-1', float_format='%.0f', index=False)

def main():
    """
    This is the main function that calls the other functions in the correct order.
    """
    # Step 1: Trim the file
    input_file = 'C://tmp//mes-puissances-atteintes-30min-007008795256-49430.csv'
    output_file = 'C://tmp//trimmed.csv'
    trim_csv(input_file, output_file)
    print(f'Cleaning and triming the file | The output file is: {output_file}')

    # Step 2: Remove duplicates
    input_file = output_file
    output_file = 'C://tmp//no_duplicates.csv'
    remove_duplicates(input_file, output_file)
    print(f'Removing duplicates | The output file is: {output_file}')

    # Step 3: Transpose the data
    input_file = output_file
    output_file = 'C://tmp//formatted.csv'
    format_data(input_file, output_file)
    print(f'Formatting date and time | The output file is: {output_file}')

if __name__ == '__main__':
    main()
