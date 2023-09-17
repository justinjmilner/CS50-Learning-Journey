
from cs50 import SQL
from flask import Flask, flash, jsonify, redirect, render_template, request, session

# Configure application
app = Flask(__name__)

# Ensure templates are auto-reloaded
app.config["TEMPLATES_AUTO_RELOAD"] = True

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///birthdays.db")


@app.after_request
def after_request(response):
    """Ensure responses aren't cached"""
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response

# Configure the homepage route
@app.route("/", methods=["GET", "POST"])
def index():
    if request.method == "POST":
        # Get the input values from the html form
        name = request.form.get("name")
        month = request.form.get("month")
        day = request.form.get("day")

        # Make an SQL query that stored the information into a database
        db.execute("INSERT INTO birthdays (name, month, day) VALUES (?, ?, ?)", name, month, day)

        # Return back to the homepage
        return redirect("/")

    else:

        # Make an SQL query to reveal all the data from birthdays
        birthdays = db.execute("SELECT * FROM birthdays")

        # Send the data to the html page to display the names with dates
        return render_template("index.html", birthdays=birthdays)

@app.route("/delete", methods=["POST"])
def delete():

    # Get the input id from the html form
    id = request.form.get("id")

    # Make an SQL query to delete the birthday from the db
    db.execute("DELETE FROM birthdays WHERE id = ?", id)

    return redirect("/")

@app.route("/edit", methods=["POST"])
def edit():

    # Get the input information from the html form
    id = request.form.get("id")
    name = request.form.get("name")
    month = request.form.get("month")
    day = request.form.get("day")

    # Make an SQL query to update the birthday from the db
    db.execute("UPDATE birthdays SET name = ?, month = ?, day = ? WHERE id = ?", name, month, day, id)

    return redirect("/")

