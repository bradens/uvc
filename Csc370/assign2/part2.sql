CREATE TABLE Parts (
    pid integer,
    pname varchar(40),
    color varchar(20),
    PRIMARY KEY(pid)
);

CREATE TABLE PartsHistory (
    pid integer,
    pname varchar(40),
    color varchar(20),
    "Operation" char(1) NOT NULL,
    "When" TIMESTAMP NOT NULL,
    "User" CHAR(20) NOT NULL,
    PRIMARY KEY(pid)
);

INSERT INTO Parts Values(1, 'wrench', 'blue');
INSERT INTO Parts Values(2, 'wrench', 'red');
INSERT INTO Parts Values(3, 'key', 'yellow');

CREATE OR REPLACE FUNCTION rowChanged(pid integer, pname VARCHAR(40), color VARCHAR(20), mode CHAR(1)) RETURNS BOOLEAN
AS '
BEGIN
    INSERT INTO PartsHistory VALUES(pid, pname, color, mode, now(), CURRENT_USER);
    RETURN TRUE;
END;
'
LANGUAGE plpgsql;


CREATE TRIGGER check_update 
    after update ON Parts
    FOR EACH ROW 
    EXECUTE PROCEDURE INSERT INTO PartsHistory VALUES(NEW.pid, NEW.pname, NEW.color, 'U', now(), CURRENT_USER);