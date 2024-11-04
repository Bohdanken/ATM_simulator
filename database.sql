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

-- Insert clients
INSERT INTO client (user_name, name, email) VALUES
('jdoe', 'John Doe', 'johndoe@example.com'),
('asmith', 'Alice Smith', 'alicesmith@example.com'),
('bjackson', 'Bob Jackson', 'bobjackson@example.com');

-- Insert accounts
INSERT INTO account (client_id, number, balance) VALUES
(1, 1000001, 1500.00),
(1, 1000002, 2500.50),
(2, 1000003, 3000.75),
(3, 1000004, 500.25);

-- Insert cards
INSERT INTO card (account_id, number, pin) VALUES
(1, 4000000000000001, 1234),
(1, 4000000000000002, 5678),
(2, 4000000000000003, 9012),
(3, 4000000000000004, 3456),
(4, 4000000000000005, 7890);

-- Insert transfers
INSERT INTO transfer (from_account_id, to_account_id, amount, datetime) VALUES
(1, 2, 200.00, '2023-01-01 10:00:00'),
(2, 3, 150.00, '2023-01-02 11:30:00'),
(3, 4, 50.00, '2023-01-03 09:45:00'),
(1, 4, 500.00, '2023-01-04 14:20:00');
