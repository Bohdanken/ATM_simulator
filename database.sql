-- Flush tables
TRUNCATE TABLE transfer RESTART IDENTITY CASCADE;
TRUNCATE TABLE card RESTART IDENTITY CASCADE;
TRUNCATE TABLE account RESTART IDENTITY CASCADE;
TRUNCATE TABLE client RESTART IDENTITY CASCADE;

-- Re-create tables
CREATE TABLE IF NOT EXISTS client (
    id BIGSERIAL PRIMARY KEY,
    user_name VARCHAR(50) NOT NULL UNIQUE,
    name VARCHAR(255) NOT NULL,
    email VARCHAR(255) NOT NULL UNIQUE
);

CREATE TABLE IF NOT EXISTS account (
    id BIGSERIAL PRIMARY KEY,
    client_id BIGINT NOT NULL REFERENCES client(id) ON DELETE CASCADE,
    number BIGINT NOT NULL UNIQUE,
    balance BIGINT NOT NULL DEFAULT 0
);

CREATE TABLE IF NOT EXISTS card (
    id BIGSERIAL PRIMARY KEY,
    account_id BIGINT NOT NULL REFERENCES account(id) ON DELETE CASCADE,
    number BIGINT NOT NULL UNIQUE,
    pin INT NOT NULL
);

CREATE TABLE IF NOT EXISTS transfer (
    id BIGSERIAL PRIMARY KEY,
    from_account_id BIGINT NOT NULL REFERENCES account(id) ON DELETE CASCADE,
    to_account_id BIGINT NOT NULL REFERENCES account(id) ON DELETE CASCADE,
    amount NUMERIC(15, 2) NOT NULL CHECK (amount > 0),
    datetime TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP,
    CHECK (from_account_id <> to_account_id)
);


INSERT INTO client (id, user_name, name, email) VALUES
(1, 'jdoe', 'John Doe', 'johndoe@example.com'),
(2, 'asmith', 'Alice Smith', 'alicesmith@example.com'),
(3, 'bjackson', 'Bob Jackson', 'bobjackson@example.com'),
(4, 'cwhite', 'Charlie White', 'charliewhite@example.com'),
(5, 'dgreen', 'Diana Green', 'dianagreen@example.com');

-- Reset client sequence
SELECT setval(pg_get_serial_sequence('client', 'id'), (SELECT MAX(id) FROM client));

-- Insert accounts
INSERT INTO account (id, client_id, number, balance) VALUES
(1, 1, 1000001, 1500.00),
(2, 1, 1000002, 2500.50),
(3, 2, 1000003, 3000.75),
(4, 3, 1000004, 500.25),
(5, 4, 1000005, 2000.00),
(6, 5, 1000006, 3500.00),
(7, 5, 1000007, 500.00);  -- Ensure this matches the `account_id` in the `card` table

-- Reset account sequence
SELECT setval(pg_get_serial_sequence('account', 'id'), (SELECT MAX(id) FROM account));

-- Insert cards
INSERT INTO card (id, account_id, number, pin) VALUES
(1, 1, 4000000000000001, 1234),
(2, 2, 4000000000000002, 5678),
(3, 3, 4000000000000003, 9012),
(4, 4, 4000000000000004, 3456),
(5, 5, 4000000000000005, 7890),
(6, 6, 4000000000000006, 1122),
(7, 7, 4000000000000007, 3344);  -- Ensure account_id = 7 exists in the `account` table

-- Reset card sequence
SELECT setval(pg_get_serial_sequence('card', 'id'), (SELECT MAX(id) FROM card));

-- Insert transfers
INSERT INTO transfer (from_account_id, to_account_id, amount, datetime) VALUES
(1, 2, 200.00, '2023-01-01 10:00:00'),
(2, 3, 150.00, '2023-01-02 11:30:00'),
(3, 4, 50.00, '2023-01-03 09:45:00'),
(5, 6, 500.00, '2023-01-05 12:00:00'),
(6, 7, 200.00, '2023-01-06 13:15:00');



