-- A log of any SQL queries used to solve the mystery

-- Get the description for the crime scene report
SELECT description
  FROM crime_scene_reports
 WHERE year = 2021
   AND month = 7
   AND day = 28
   AND street = 'Humphrey Street';
/* Tells us the 3 witnesses used the word bakery, the time of the crime
was 10:15am, the location was Humphrey Street Bakery*/


-- Get interview information
SELECT transcript, name
  FROM interviews
 WHERE year = 2021
   AND month = 7
   AND DAY = 28
   AND transcript LIKE ('%bakery%');
/* Witness 1: Exit, 10:15am to 10:25am. Witness 2: ATM withdraw, Leggett Street.
Witness 3: Phone call < 60 seconds, earliest flight out of Fiftyville on 29, callee is accomplice.*/


-- Following tables are data which contain the culprit information --

-- License plates
SELECT license_plate
  FROM bakery_security_logs
 WHERE hour = 10
   AND minute BETWEEN 15 AND 25
   AND activity = 'exit';


-- Bank account numbers
SELECT account_number, amount
  FROM atm_transactions
 WHERE year = 2021
   AND month = 7
   AND day = 28
   AND atm_location = 'Leggett Street'
   AND transaction_type = 'withdraw';


-- Phone numbers
SELECT caller, receiver
  FROM phone_calls
 WHERE year = 2021
   AND month = 7
   AND day = 28
   AND duration < 60;


-- Flight id, and escape destination id
SELECT id, origin_airport_id, destination_airport_id
  FROM flights
 WHERE year = 2021
   AND month = 7
   AND day = 29
   AND hour = 8
   AND minute = 20;
--  id 36 | origin_airport_id 8 | destination_airport_id 4


-- Escape destination name
SELECT abbreviation, full_name, city
  FROM airports
 WHERE id = 4;
/* CSF | Fiftyville Regional Airport | Fiftyville    ...TO...
LGA | LaGuardia Airport | New York City */


-- List of passport numbers
SELECT passport_number, seat
  FROM passengers
 WHERE flight_id = 36;


-- List of person id's
SELECT person_id, creation_year
  FROM bank_accounts
 WHERE account_number IN
       (SELECT account_number
       FROM atm_transactions
       WHERE year = 2021
       AND month = 7
       AND day = 28
       AND atm_location = 'Leggett Street'
       AND transaction_type = 'withdraw');


-- Get name
/* The person will have used a phone number from the evidence, have an id in bank accounts id's from the transaction,
a passport number from passengers on board, and a license plate from security footage. */
SELECT name
  FROM people
 WHERE phone_number IN
       (SELECT caller
       FROM phone_calls
       WHERE year = 2021
       AND month = 7
       AND day = 28
       AND duration < 60)
   AND license_plate IN
       (SELECT license_plate
       FROM bakery_security_logs
       WHERE hour = 10
       AND minute BETWEEN 15 AND 25
       AND activity = 'exit')
   AND passport_number IN
       (SELECT passport_number
       FROM passengers
       WHERE flight_id = 36)
   AND id IN
       (SELECT person_id
       FROM bank_accounts
       WHERE account_number IN
                            (SELECT account_number
                            FROM atm_transactions
                            WHERE year = 2021
                            AND month = 7
                            AND day = 28
                            AND atm_location = 'Leggett Street'
                            AND transaction_type = 'withdraw'));


-- Get phone number of culprit
/* The accomplice will be the receiver of the culprits phone number
from the list of phone numbers made during the reported call period. */
SELECT name
  FROM people
 WHERE phone_number IN
       (SELECT receiver
       FROM phone_calls
       WHERE year = 2021
       AND month = 7
       AND day = 28
       AND duration < 60
       AND caller =
          (SELECT phone_number
          FROM people
          WHERE id = 686048));


