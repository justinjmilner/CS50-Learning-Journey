import os

from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session
from flask_session import Session
from tempfile import mkdtemp
from werkzeug.security import check_password_hash, generate_password_hash

from helpers import apology, login_required, lookup, usd

import random
from datetime import datetime

# Configure application
app = Flask(__name__)

# Custom filter
app.jinja_env.filters["usd"] = usd

# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")

# Make sure API key is set
if not os.environ.get("API_KEY"):
    raise RuntimeError("API_KEY not set")


@app.after_request
def after_request(response):
    """Ensure responses aren't cached"""
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response


@app.route("/")
@login_required
def index():
    """Show portfolio of stocks"""

    # Query for sum of shares owned
    userid = session["user_id"]
    stocks = db.execute("SELECT symbol, shares FROM purchases WHERE userid = ?", userid)

    # Calculate current value of stocks owned and lookup stock data
    stockinfo = []
    values = []
    for i in range(0, len(stocks)):
        stockname = lookup(stocks[i]['symbol'])
        currentprice = stockname['price']
        quantity = stocks[i]['shares']
        values.append(currentprice * quantity)
        stockinfo.append(lookup(stocks[i]['symbol']))

    # Update shares in stockinfo
    for i in range(0, len(stockinfo)):
        stockinfo[i]['shares'] = stocks[i]['shares']

    # Calculate cash balance and total balance
    balance = db.execute("SELECT cash FROM users WHERE id = ?", userid)
    balance = balance[0]['cash']
    total = balance + sum(values)
    stockbalance = sum(values)

    # Calculate PNL
    pnl = total - 10000

    # Convert to usd format
    total = usd(total)
    balance = usd(balance)
    pnl = usd(pnl)
    stockbalance = usd(stockbalance)
    for i in range(0, len(stockinfo)):
        values[i] = usd(values[i])
        stockinfo[i]['value'] = values[i]

    return render_template("index.html", stockinfo=stockinfo, pnl=pnl, balance=balance, total=total, stockbalance=stockbalance)


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock"""

    if request.method == "POST":

        # Check for valid symbol input
        if not request.form.get("symbol"):
            return apology("enter stock symbol", 403)

        # Confirm symbol is in database
        symbol = request.form.get("symbol")
        if not lookup(symbol):
            return apology("invalid stock symbol", 403)

        # Retrieve valid stock quanitty
        if not request.form.get("shares"):
            return apology("enter number of shares", 403)

        shares = int(request.form.get("shares"))
        if shares < 1:
            return apology("invalid number of shares", 403)

        # Fetch stock price information from API
        stockinfo = lookup(symbol)
        price = stockinfo['price']
        cost = shares * price

        # Confirm user has the funds to purchase
        userid = session["user_id"]
        balance = db.execute("SELECT cash FROM users WHERE id = ?", userid)
        balance = balance[0]['cash']
        if cost > balance:
            return apology("insufficient balance", 403)

        # Calculate new cash balance
        balance = balance - cost
        db.execute("UPDATE users SET cash = ? WHERE id = ?", balance, userid)

        # Store purchase information in a database
        exists = db.execute("SELECT shares FROM purchases WHERE symbol = ?", symbol)
        if exists:
            currentshares = db.execute("SELECT shares FROM purchases WHERE symbol = ?", symbol)
            number = shares + int(currentshares[0]['shares'])
            db.execute("UPDATE purchases SET shares = ? WHERE symbol = ?", number, symbol)
        else:
            db.execute("INSERT INTO purchases (userid, shares, cost, symbol) VALUES (?, ?, ?, ?)", userid, shares, cost, symbol)

        # Get current time of purchase
        now = datetime.now()
        time = now.strftime("%m/%d/%Y, %H:%M:%S")

        # Convert to usd
        cost = usd(cost)

        # Store history information in a database
        type = 'buy'
        db.execute("INSERT INTO history (userid, price, shares, cost, symbol, time, type) VALUES (?, ?, ?, ?, ?, ?, ?)",
                   userid, price, shares, cost, symbol, time, type)

        # Redirect user to homepage
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("buy.html")


@app.route("/history")
@login_required
def history():
    """Show history of transactions"""

    # Query for current users password
    userid = session["user_id"]

    history = db.execute("SELECT * FROM history WHERE userid = ?", userid)

    # Direct the user to the history page
    return render_template("history.html", history=history)


@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in"""

    # Forget any user_id
    session.clear()

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username", 403)

        # Ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password", 403)

        # Query database for username
        rows = db.execute("SELECT * FROM users WHERE username = ?", request.form.get("username"))

        # Ensure username exists and password is correct
        if len(rows) != 1 or not check_password_hash(rows[0]["hash"], request.form.get("password")):
            return apology("invalid username and/or password", 403)

        # Remember which user has logged in
        session["user_id"] = rows[0]["id"]

        # Redirect user to home page
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("login.html")


@app.route("/logout")
def logout():
    """Log user out"""

    # Forget any user_id
    session.clear()

    # Redirect user to login form
    return redirect("/")


@app.route("/quote", methods=["GET", "POST"])
@login_required
def quote():
    """Get stock quote."""

    if request.method == "POST":

        # Check for symbol input
        if not request.form.get("symbol"):
            return apology("enter stock symbol", 403)

        # Check symbol is in database
        symbol = request.form.get("symbol")

        if not lookup(symbol):
            return apology("invalid stock symbol", 403)

        # Fetch stock information and store in a dictionary
        stockinfo = lookup(symbol)

        # Convert float to dollar value
        value = usd(stockinfo['price'])
        stockinfo['price'] = value

        # Render the page for the client with the stock information on it
        return render_template("quoted.html", stockinfo=stockinfo)

    # On initial get request, load register page
    else:
        return render_template("quote.html")


@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user"""

    if request.method == "POST":

        # Check for username entry usage
        if not request.form.get("username"):
            return apology("must provide username", 403)

        # Get username from html form
        username = request.form.get("username")

        # Query if the username already exists in the database
        exists = db.execute("SELECT username FROM users WHERE username = ?", username)

        if exists:
            return apology("must provide unique username", 403)

        # Check for password entries usage
        if not request.form.get("password"):
            return apology("must provide password", 403)

        if not request.form.get("confirmation"):
            return apology("must provide confirmation password", 403)

        # Get password entries from html form
        password = request.form.get("password")
        confirmation = request.form.get("confirmation")

        # Check password confirmation matches
        if password != confirmation:
            return apology("passwords don't match", 403)

        # Generate a user id
        idnum = db.execute("SELECT * FROM users")
        idnum = len(idnum)
        idnum = checkid(idnum)

        # Hash password
        pwhash = generate_password_hash(password)

        # Store user info in the database
        db.execute("INSERT INTO users (id, username, hash) VALUES (?, ?, ?)", idnum, username, pwhash)

        return redirect("/login")

    # On initial get request, load register page
    else:
        return render_template("register.html")


# Check id doesnt exist in db
def checkid(int):

    # If initial id number is taken then a random number is assigned
    checkidnum = int
    idexists = db.execute("SELECT id FROM users WHERE id = ?", checkidnum)

    if idexists:
        checkidnum = random.randint(0, 10000000000000000000)
        checkid(checkidnum)
    else:
        return checkidnum


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock"""

    if request.method == "POST":

        # Check for proper sell input
        if not request.form.get("symbol"):
            return apology("input symbol", 403)

        if not request.form.get("shares"):
            return apology("input shares", 403)

        # Get information for the history
        shares = int(request.form.get("shares"))
        userid = session["user_id"]
        symbol = request.form.get("symbol")
        info = lookup(symbol)
        price = info['price']
        cost = shares * price
        addcash = cost
        cost = -abs(shares * price)
        cost = usd(cost)

        # Get the current number of shares of the desired stock to sell
        sum = db.execute("SELECT shares FROM purchases WHERE symbol = ? AND userid = ?", symbol, userid)
        sum = int(sum[0]['shares'])

        # Check for sufficient shares to sell
        newsum = sum - shares
        if newsum < 0:
            return apology("insufficient shares", 403)

        # Update new sum of owned shares
        db.execute("UPDATE purchases SET shares = ? WHERE symbol = ?", newsum, symbol)

        # Update cash balance
        cash = db.execute("SELECT cash FROM users WHERE id = ?", userid)
        cash = cash[0]['cash']
        cash += addcash
        db.execute("UPDATE users SET cash = ? WHERE id = ?", cash, userid)

        # Get current time of sell
        now = datetime.now()
        time = now.strftime("%m/%d/%Y, %H:%M:%S")

        # Input sell information into history
        type = 'sell'
        db.execute("INSERT INTO history (userid, price, shares, cost, symbol, time, type) VALUES (?, ?, ?, ?, ?, ?, ?)",
                   userid, price, shares, cost, symbol, time, type)

        return redirect("/")

    else:
        return render_template("sell.html")


@app.route("/verifypw", methods=["GET", "POST"])
@login_required
def verifypw():

    if request.method == "POST":

        # Check for proper form usage
        if not request.form.get("password"):
            return apology("enter valid password", 403)

        # Query for current users password
        userid = session["user_id"]

        # Check if they match
        password = db.execute("SELECT hash FROM users WHERE id = ?", userid)

        if not check_password_hash(password[0]['hash'], request.form.get("password")):
            return apology("invalid password", 403)

        return render_template("changepw.html")

    else:
        return render_template("verifypw.html")


@app.route("/changepw", methods=["GET", "POST"])
@login_required
def changepw():

    if request.method == "POST":

        # Check for proper form use
        if not request.form.get("password"):
            return apology("enter valid password", 403)
        if not request.form.get("confirmation"):
            return apology("enter valid password", 403)

        # Compare inputs match
        password1 = request.form.get("password")
        password2 = request.form.get("confirmation")

        if password1 != password2:
            return apology("passwords don't match", 403)

        # Update new password
        hashpw = generate_password_hash(password1)
        userid = session["user_id"]
        db.execute("UPDATE users SET hash = ? WHERE id = ?", hashpw, userid)

        return redirect("/")

    else:
        return render_template("changepw.html")


@app.route("/addcash", methods=["GET", "POST"])
@login_required
def addcash():

    if request.method == "POST":

        # Check for proper form usage
        if not request.form.get("addcash"):
            return apology("invalid amount", 403)

        cash = int(request.form.get("addcash"))

        if cash < 0:
            return apology("invalid amount", 403)

        # Get current users balance
        userid = session["user_id"]
        balance = db.execute("SELECT cash FROM users WHERE id = ?", userid)
        balance = int(balance[0]['cash'])

        # Update cash balance
        balance += cash
        db.execute("UPDATE users SET cash = ? WHERE id = ?", balance, userid)

        return redirect("/")

    else:
        return render_template("addcash.html")