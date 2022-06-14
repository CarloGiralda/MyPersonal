-- MySQL Workbench Forward Engineering

SET @OLD_UNIQUE_CHECKS=@@UNIQUE_CHECKS, UNIQUE_CHECKS=0;
SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0;
SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='ONLY_FULL_GROUP_BY,STRICT_TRANS_TABLES,NO_ZERO_IN_DATE,NO_ZERO_DATE,ERROR_FOR_DIVISION_BY_ZERO,NO_ENGINE_SUBSTITUTION';

-- -----------------------------------------------------
-- Schema mydb
-- -----------------------------------------------------
-- -----------------------------------------------------
-- Schema azienda
-- -----------------------------------------------------

-- -----------------------------------------------------
-- Schema azienda
-- -----------------------------------------------------
CREATE SCHEMA IF NOT EXISTS `azienda` DEFAULT CHARACTER SET utf8mb4 COLLATE utf8mb4_0900_ai_ci ;
USE `azienda` ;

-- -----------------------------------------------------
-- Table `azienda`.`Dipendente`
-- -----------------------------------------------------
CREATE TABLE IF NOT EXISTS `azienda`.`Dipendente` (
  `ID` INT NOT NULL AUTO_INCREMENT,
  `Username` VARCHAR(45) NOT NULL,
  `Password` VARCHAR(45) NOT NULL,
  `Ruolo` ENUM('funzionario', 'manager', 'commerciale') NOT NULL,
  PRIMARY KEY (`ID`),
  UNIQUE INDEX `Username_UNIQUE` (`Username` ASC) VISIBLE)
ENGINE = InnoDB
AUTO_INCREMENT = 25
DEFAULT CHARACTER SET = utf8mb4
COLLATE = utf8mb4_0900_ai_ci;


-- -----------------------------------------------------
-- Table `azienda`.`Cliente`
-- -----------------------------------------------------
CREATE TABLE IF NOT EXISTS `azienda`.`Cliente` (
  `CodiceFiscale` VARCHAR(128) NOT NULL,
  `Nome` VARCHAR(45) NOT NULL,
  `Cognome` VARCHAR(45) NOT NULL,
  `DataDiNascita` DATE NOT NULL,
  `DataDiRegistrazione` DATE NOT NULL,
  `Fax` INT NULL DEFAULT NULL,
  `Email` VARCHAR(128) NULL DEFAULT NULL,
  `Indirizzo` VARCHAR(128) NULL DEFAULT NULL,
  `RecapitoTelefonico` INT NULL DEFAULT NULL,
  `IDComm` INT NULL DEFAULT NULL,
  `IDFunz` INT NULL DEFAULT NULL,
  `PartitaIVA` VARCHAR(45) NULL DEFAULT NULL,
  PRIMARY KEY (`CodiceFiscale`),
  INDEX `fk_ID_Comm_idx` (`IDComm` ASC) VISIBLE,
  INDEX `fk_ID_Funz_idx` (`IDFunz` ASC) VISIBLE,
  CONSTRAINT `fk_Cliente_1`
    FOREIGN KEY (`IDComm`)
    REFERENCES `azienda`.`Dipendente` (`ID`)
    ON DELETE SET NULL
    ON UPDATE CASCADE,
  CONSTRAINT `fk_Cliente_2`
    FOREIGN KEY (`IDFunz`)
    REFERENCES `azienda`.`Dipendente` (`ID`)
    ON DELETE SET NULL
    ON UPDATE CASCADE)
ENGINE = InnoDB
DEFAULT CHARACTER SET = utf8mb4
COLLATE = utf8mb4_0900_ai_ci;


-- -----------------------------------------------------
-- Table `azienda`.`PropostaCommerciale`
-- -----------------------------------------------------
CREATE TABLE IF NOT EXISTS `azienda`.`PropostaCommerciale` (
  `CodiceAlfanumerico` VARCHAR(128) NOT NULL,
  `DataDiAttivazione` DATE NULL DEFAULT NULL,
  `DataDiScadenza` DATE NULL DEFAULT NULL,
  `Validità` ENUM('Attiva', 'Terminata') NOT NULL,
  PRIMARY KEY (`CodiceAlfanumerico`))
ENGINE = InnoDB
DEFAULT CHARACTER SET = utf8mb4
COLLATE = utf8mb4_0900_ai_ci;


-- -----------------------------------------------------
-- Table `azienda`.`Acquisto`
-- -----------------------------------------------------
CREATE TABLE IF NOT EXISTS `azienda`.`Acquisto` (
  `Cliente` VARCHAR(128) NOT NULL,
  `CodiceProposta` VARCHAR(128) NOT NULL,
  PRIMARY KEY (`Cliente`, `CodiceProposta`),
  INDEX `fk_Cliente_idx` (`Cliente` ASC) VISIBLE,
  INDEX `fk_PropostaCommerciale_idx` (`CodiceProposta` ASC) VISIBLE,
  CONSTRAINT `fk_Cliente`
    FOREIGN KEY (`Cliente`)
    REFERENCES `azienda`.`Cliente` (`CodiceFiscale`),
  CONSTRAINT `fk_PropostaCommerciale`
    FOREIGN KEY (`CodiceProposta`)
    REFERENCES `azienda`.`PropostaCommerciale` (`CodiceAlfanumerico`)
    ON DELETE RESTRICT
    ON UPDATE RESTRICT)
ENGINE = InnoDB
DEFAULT CHARACTER SET = utf8mb4
COLLATE = utf8mb4_0900_ai_ci;


-- -----------------------------------------------------
-- Table `azienda`.`Sede`
-- -----------------------------------------------------
CREATE TABLE IF NOT EXISTS `azienda`.`Sede` (
  `Indirizzo` VARCHAR(128) NOT NULL,
  `SalaRiunione` VARCHAR(45) NOT NULL,
  PRIMARY KEY (`Indirizzo`))
ENGINE = InnoDB
DEFAULT CHARACTER SET = utf8mb4
COLLATE = utf8mb4_0900_ai_ci;


-- -----------------------------------------------------
-- Table `azienda`.`Appuntamento`
-- -----------------------------------------------------
CREATE TABLE IF NOT EXISTS `azienda`.`Appuntamento` (
  `Giorno` DATE NOT NULL,
  `Ora` TIME NOT NULL,
  `IndirizzoSede` VARCHAR(128) NOT NULL,
  `AdesioneFunzionario` INT NOT NULL,
  `PartecipazioneCliente` VARCHAR(128) NOT NULL,
  `ID` INT NOT NULL AUTO_INCREMENT,
  `NotaDescrittiva` VARCHAR(4096) NOT NULL,
  PRIMARY KEY (`ID`),
  UNIQUE INDEX `UNIQUE` (`Giorno` ASC, `Ora` ASC, `IndirizzoSede` ASC) VISIBLE,
  INDEX `fk_Appuntamento_idx` (`AdesioneFunzionario` ASC) VISIBLE,
  INDEX `fk_ID_Cliente_idx` (`PartecipazioneCliente` ASC) VISIBLE,
  INDEX `fk_Sede_idx` (`IndirizzoSede` ASC) VISIBLE,
  CONSTRAINT `fk_ID_Cliente`
    FOREIGN KEY (`PartecipazioneCliente`)
    REFERENCES `azienda`.`Cliente` (`CodiceFiscale`)
    ON DELETE RESTRICT
    ON UPDATE CASCADE,
  CONSTRAINT `fk_ID_Dipendente`
    FOREIGN KEY (`AdesioneFunzionario`)
    REFERENCES `azienda`.`Dipendente` (`ID`),
  CONSTRAINT `fk_Sede`
    FOREIGN KEY (`IndirizzoSede`)
    REFERENCES `azienda`.`Sede` (`Indirizzo`))
ENGINE = InnoDB
AUTO_INCREMENT = 87
DEFAULT CHARACTER SET = utf8mb4
COLLATE = utf8mb4_0900_ai_ci;


-- -----------------------------------------------------
-- Table `azienda`.`Contatto`
-- -----------------------------------------------------
CREATE TABLE IF NOT EXISTS `azienda`.`Contatto` (
  `ID` INT NOT NULL AUTO_INCREMENT,
  `ClienteContattato` VARCHAR(128) NOT NULL,
  `FunzionarioContattante` INT NOT NULL,
  PRIMARY KEY (`ID`),
  INDEX `fk_Dipendente_idx` (`FunzionarioContattante` ASC) VISIBLE,
  INDEX `fk_Cliente_idx` (`ClienteContattato` ASC) VISIBLE,
  CONSTRAINT `fk_Cliente_idx`
    FOREIGN KEY (`ClienteContattato`)
    REFERENCES `azienda`.`Cliente` (`CodiceFiscale`),
  CONSTRAINT `fk_Funzionario_idx`
    FOREIGN KEY (`FunzionarioContattante`)
    REFERENCES `azienda`.`Dipendente` (`ID`))
ENGINE = InnoDB
AUTO_INCREMENT = 73
DEFAULT CHARACTER SET = utf8mb4
COLLATE = utf8mb4_0900_ai_ci;


-- -----------------------------------------------------
-- Table `azienda`.`Correlato`
-- -----------------------------------------------------
CREATE TABLE IF NOT EXISTS `azienda`.`Correlato` (
  `Cliente` VARCHAR(128) NOT NULL,
  `CodiceProposta` VARCHAR(128) NOT NULL,
  PRIMARY KEY (`Cliente`, `CodiceProposta`),
  INDEX `fk_Cliente_idx` (`Cliente` ASC) VISIBLE,
  INDEX `fk_PropostaCliente_idx` (`CodiceProposta` ASC) VISIBLE,
  CONSTRAINT `fk_Client_idx`
    FOREIGN KEY (`Cliente`)
    REFERENCES `azienda`.`Cliente` (`CodiceFiscale`),
  CONSTRAINT `fk_PropostaCliente`
    FOREIGN KEY (`CodiceProposta`)
    REFERENCES `azienda`.`PropostaCommerciale` (`CodiceAlfanumerico`))
ENGINE = InnoDB
DEFAULT CHARACTER SET = utf8mb4
COLLATE = utf8mb4_0900_ai_ci;


-- -----------------------------------------------------
-- Table `azienda`.`Gestione`
-- -----------------------------------------------------
CREATE TABLE IF NOT EXISTS `azienda`.`Gestione` (
  `Manager` INT NOT NULL,
  `CodiceProposta` VARCHAR(128) NOT NULL,
  PRIMARY KEY (`Manager`, `CodiceProposta`),
  UNIQUE INDEX `CodiceProposta_UNIQUE` (`CodiceProposta` ASC) VISIBLE,
  CONSTRAINT `fk_Manager`
    FOREIGN KEY (`Manager`)
    REFERENCES `azienda`.`Dipendente` (`ID`),
  CONSTRAINT `fk_Proposta`
    FOREIGN KEY (`CodiceProposta`)
    REFERENCES `azienda`.`PropostaCommerciale` (`CodiceAlfanumerico`))
ENGINE = InnoDB
DEFAULT CHARACTER SET = utf8mb4
COLLATE = utf8mb4_0900_ai_ci;


-- -----------------------------------------------------
-- Table `azienda`.`Nota`
-- -----------------------------------------------------
CREATE TABLE IF NOT EXISTS `azienda`.`Nota` (
  `Numero` INT NOT NULL,
  `Funzionario` INT NOT NULL,
  `Testo` VARCHAR(4096) NOT NULL,
  `ContattoAssociato` INT NOT NULL,
  PRIMARY KEY (`Numero`),
  INDEX `fk_Dipendente_idx` (`Funzionario` ASC) VISIBLE,
  INDEX `fk_Contatto_idx` (`ContattoAssociato` ASC) VISIBLE,
  CONSTRAINT `fk_Nota_1`
    FOREIGN KEY (`Funzionario`)
    REFERENCES `azienda`.`Dipendente` (`ID`),
  CONSTRAINT `fk_Nota_2`
    FOREIGN KEY (`ContattoAssociato`)
    REFERENCES `azienda`.`Contatto` (`ID`))
ENGINE = InnoDB
DEFAULT CHARACTER SET = utf8mb4
COLLATE = utf8mb4_0900_ai_ci;


-- -----------------------------------------------------
-- Table `azienda`.`Relazione`
-- -----------------------------------------------------
CREATE TABLE IF NOT EXISTS `azienda`.`Relazione` (
  `Nota` INT NOT NULL,
  `CodiceProposta` VARCHAR(128) NOT NULL,
  PRIMARY KEY (`Nota`, `CodiceProposta`),
  INDEX `fk_Nota_idx` (`Nota` ASC) VISIBLE,
  INDEX `fk_Relazione_2_idx` (`CodiceProposta` ASC) VISIBLE,
  CONSTRAINT `fk_Relazione_1`
    FOREIGN KEY (`Nota`)
    REFERENCES `azienda`.`Nota` (`Numero`),
  CONSTRAINT `fk_Relazione_2`
    FOREIGN KEY (`CodiceProposta`)
    REFERENCES `azienda`.`PropostaCommerciale` (`CodiceAlfanumerico`))
ENGINE = InnoDB
DEFAULT CHARACTER SET = utf8mb4
COLLATE = utf8mb4_0900_ai_ci;

USE `azienda` ;

-- -----------------------------------------------------
-- Placeholder table for view `azienda`.`proposte_terminate`
-- -----------------------------------------------------
CREATE TABLE IF NOT EXISTS `azienda`.`proposte_terminate` (`CodiceAlfanumerico` INT);

-- -----------------------------------------------------
-- procedure agenda
-- -----------------------------------------------------

DELIMITER $$
USE `azienda`$$
CREATE DEFINER=`root`@`localhost` PROCEDURE `agenda`(in ID INT)
BEGIN	declare exit handler for sqlexception
    begin
        rollback;
        resignal;
    end;
    
    set transaction isolation level read committed;
	start transaction;
		select Appuntamento.Giorno,Appuntamento.Ora,Appuntamento.IndirizzoSede from Appuntamento where Appuntamento.AdesioneFunzionario = ID;
	commit;
END$$

DELIMITER ;

-- -----------------------------------------------------
-- procedure eliminare_nota
-- -----------------------------------------------------

DELIMITER $$
USE `azienda`$$
CREATE DEFINER=`root`@`localhost` PROCEDURE `eliminare_nota`(in ID INT)
BEGIN
	declare exit handler for sqlexception
    begin
        rollback;
        resignal;
    end;
    
    set transaction isolation level read committed;

	start transaction;
		delete from Relazione where Relazione.Nota = ID;
		delete from Nota where Nota.Numero = ID;
	commit;
END$$

DELIMITER ;

-- -----------------------------------------------------
-- procedure inserire_appuntamento
-- -----------------------------------------------------

DELIMITER $$
USE `azienda`$$
CREATE DEFINER=`root`@`localhost` PROCEDURE `inserire_appuntamento`(in giorno DATE, in ora TIME, in sede varchar(128), in func INT, in cliente varchar(128), in nota varchar(4096), out codice_appuntamento INT)
BEGIN
	declare exit handler for sqlexception
    begin
        rollback;
        resignal;
    end;

	set transaction isolation level repeatable read;
    start transaction;
		insert into `Appuntamento` (`Giorno`, `Ora`, `IndirizzoSede`, `AdesioneFunzionario`, `PartecipazioneCliente`, `NotaDescrittiva`) 
			values (giorno, ora, sede, func, cliente, nota); 
	commit;

     set codice_appuntamento = last_insert_id(); 
END$$

DELIMITER ;

-- -----------------------------------------------------
-- procedure inserire_cliente
-- -----------------------------------------------------

DELIMITER $$
USE `azienda`$$
CREATE DEFINER=`root`@`localhost` PROCEDURE `inserire_cliente`(in cod_fisc varchar(128), in nam varchar(45), in surname varchar(45), in birthday DATE, in registration_date DATE, in fax INT, in email varchar(128), in address varchar(128), in phone INT, in comm INT, in func INT, in iva varchar(45))
BEGIN
	if(iva != '') then
		insert into `Cliente` (`CodiceFiscale`, `Nome`, `Cognome`, `DataDiNascita`, `DataDiRegistrazione`, `Fax`, `Email`, `Indirizzo`, `RecapitoTelefonico`, `IDComm`, `IDFunz`, `PartitaIVA`)
				values (cod_fisc, nam, surname, birthday, registration_date, fax, email, address, phone, comm, func, iva);
	else
		insert into `Cliente` (`CodiceFiscale`, `Nome`, `Cognome`, `DataDiNascita`, `DataDiRegistrazione`, `Fax`, `Email`, `Indirizzo`, `RecapitoTelefonico`, `IDComm`, `IDFunz`, `PartitaIVA`)
				values (cod_fisc, nam, surname, birthday, registration_date, fax, email, address, phone, comm, func, null);
	end if;
END$$

DELIMITER ;

-- -----------------------------------------------------
-- procedure inserire_consulenza
-- -----------------------------------------------------

DELIMITER $$
USE `azienda`$$
CREATE DEFINER=`root`@`localhost` PROCEDURE `inserire_consulenza`(in var_username varchar(45), in cod varchar(128), in dat DATE)
BEGIN
	declare tempID INT;

	declare exit handler for sqlexception
    begin
        rollback;
        resignal;
    end;
    
    set transaction isolation level read committed;

	start transaction;
		select `ID` from `Dipendente` where `Username` = var_username and `Ruolo` = 'manager' into tempID;
		
		insert into `PropostaCommerciale` (`CodiceAlfanumerico`, `DataDiAttivazione`, `Validità`)
			values (cod, dat,'Attiva');
		insert into `Gestione` (`Manager`, `CodiceProposta`)
			values (tempID, cod);
	commit;
END$$

DELIMITER ;

-- -----------------------------------------------------
-- procedure inserire_contatto
-- -----------------------------------------------------

DELIMITER $$
USE `azienda`$$
CREATE DEFINER=`root`@`localhost` PROCEDURE `inserire_contatto`(out codice_contatto INT, in cliente varchar(128), in funzionario INT)
BEGIN
	declare exit handler for sqlexception
    begin
        rollback;
        resignal;
    end;

	set transaction isolation level repeatable read;
    start transaction;
		insert into `Contatto` (`ClienteContattato`, `FunzionarioContattante`)
			values (cliente, funzionario);
	commit;


	set codice_contatto = last_insert_id();
END$$

DELIMITER ;

-- -----------------------------------------------------
-- procedure inserire_nota
-- -----------------------------------------------------

DELIMITER $$
USE `azienda`$$
CREATE DEFINER=`root`@`localhost` PROCEDURE `inserire_nota`(in id INT, in testo varchar(4096), in func INT, in input INT, in cod varchar(128))
BEGIN
	declare exit handler for sqlexception
    begin
        rollback;
        resignal;
    end;

	set transaction isolation level repeatable read;
	start transaction;
		insert into Nota (`Numero`, `Funzionario`, `Testo`, `ContattoAssociato`) values (id, func, testo, input);
        
        if(cod != '') then
			insert into Relazione (`Nota`, `CodiceProposta`) values (id, cod);
		end if;
	commit;
END$$

DELIMITER ;

-- -----------------------------------------------------
-- procedure inserire_opportunità
-- -----------------------------------------------------

DELIMITER $$
USE `azienda`$$
CREATE DEFINER=`root`@`localhost` PROCEDURE `inserire_opportunità`(in CodFis varchar(128), in CodPr varchar(128))
BEGIN
	insert into `Correlato` (`Cliente`, `CodiceProposta`)
		values (CodFis, CodPr);
END$$

DELIMITER ;

-- -----------------------------------------------------
-- procedure inserire_utente
-- -----------------------------------------------------

DELIMITER $$
USE `azienda`$$
CREATE DEFINER=`root`@`localhost` PROCEDURE `inserire_utente`(in username varchar(45), in pass varchar(45), out codice INT)
BEGIN
	insert into `Dipendente` (`Username`, `Password`, `Ruolo`)
		values (username, pass, 'funzionario');
	
	set codice = last_insert_id();
END$$

DELIMITER ;

-- -----------------------------------------------------
-- procedure invalidare_consulenza
-- -----------------------------------------------------

DELIMITER $$
USE `azienda`$$
CREATE DEFINER=`root`@`localhost` PROCEDURE `invalidare_consulenza`(in cod varchar(128))
BEGIN
	if (select count(*) from Correlato where `CodiceProposta` = cod) = 0 then
		update PropostaCommerciale set `Validità` = 'Terminata', `DataDiScadenza` = CURDATE(), `DataDiAttivazione` = NULL where `CodiceAlfanumerico` = cod;
	else
		signal sqlstate '45000';
    end if;
END$$

DELIMITER ;

-- -----------------------------------------------------
-- procedure login
-- -----------------------------------------------------

DELIMITER $$
USE `azienda`$$
CREATE DEFINER=`root`@`localhost` PROCEDURE `login`(in var_username varchar(45), in var_pass varchar(45), out var_role INT)
BEGIN
	declare var_user_role ENUM('funzionario', 'manager', 'commerciale');
    
    select `Ruolo` from `Dipendente`
		where `Username` = var_username
        and `Password` = var_pass
        into var_user_role;
        
	-- See the corresponding enum in the client
		if var_user_role = 'funzionario' then
			set var_role = 1;
		elseif var_user_role = 'manager' then
			set var_role = 2;
		elseif var_user_role = 'commerciale' then
			set var_role = 3;
		else
			set var_role =  4;
		end if;
END$$

DELIMITER ;

-- -----------------------------------------------------
-- procedure modificare_nota
-- -----------------------------------------------------

DELIMITER $$
USE `azienda`$$
CREATE DEFINER=`root`@`localhost` PROCEDURE `modificare_nota`(in testo varchar(4096), in id INT)
BEGIN
	update Nota set `Testo` = testo where `Numero` = id;
END$$

DELIMITER ;

-- -----------------------------------------------------
-- procedure visualizzare_cliente_acquisti
-- -----------------------------------------------------

DELIMITER $$
USE `azienda`$$
CREATE DEFINER=`root`@`localhost` PROCEDURE `visualizzare_cliente_acquisti`(in cod_fisc varchar(128))
BEGIN
	declare exit handler for sqlexception
    begin
        rollback;
        resignal;
    end;
    
    set transaction isolation level read committed;
    set transaction read only;
    start transaction;
		select CodiceProposta from Acquisto where `Cliente` = cod_fisc;
    commit;
END$$

DELIMITER ;

-- -----------------------------------------------------
-- procedure visualizzare_cliente_dati
-- -----------------------------------------------------

DELIMITER $$
USE `azienda`$$
CREATE DEFINER=`root`@`localhost` PROCEDURE `visualizzare_cliente_dati`(in cod_fisc varchar(128))
BEGIN
	declare exit handler for sqlexception
    begin
        rollback;
        resignal;
    end;
    
    set transaction isolation level read committed;
    set transaction read only;
    start transaction;
		select * from Cliente where `CodiceFiscale` = cod_fisc;
    commit;
END$$

DELIMITER ;

-- -----------------------------------------------------
-- procedure visualizzare_cliente_note
-- -----------------------------------------------------

DELIMITER $$
USE `azienda`$$
CREATE DEFINER=`root`@`localhost` PROCEDURE `visualizzare_cliente_note`(in cod_fisc varchar(128))
BEGIN
	declare exit handler for sqlexception
    begin
        rollback;
        resignal;
    end;
    
    set transaction isolation level read committed;
    set transaction read only;
    start transaction;
	    select n.Numero 
		from Nota as n left join Contatto as c on n.ContattoAssociato = c.ID
		where c.ClienteContattato = cod_fisc;
    commit;
END$$

DELIMITER ;

-- -----------------------------------------------------
-- procedure visualizzare_clienti_assegnati
-- -----------------------------------------------------

DELIMITER $$
USE `azienda`$$
CREATE DEFINER=`root`@`localhost` PROCEDURE `visualizzare_clienti_assegnati`(in ID INT)
BEGIN
	declare exit handler for sqlexception
    begin
        rollback;
        resignal;
    end;
    
    set transaction isolation level read committed;
    set transaction read only;
    start transaction;
		select `CodiceFiscale` from Cliente where Cliente.IDFunz = ID;
    commit;
END$$

DELIMITER ;

-- -----------------------------------------------------
-- procedure visualizzare_consulenze_attive
-- -----------------------------------------------------

DELIMITER $$
USE `azienda`$$
CREATE DEFINER=`root`@`localhost` PROCEDURE `visualizzare_consulenze_attive`(in cod INT)
BEGIN
	declare exit handler for sqlexception
    begin
        rollback;
        resignal;
    end;
    
    set transaction isolation level read committed;
    set transaction read only;
    start transaction;
		select `CodiceProposta` from Gestione join PropostaCommerciale on `CodiceProposta` = `CodiceAlfanumerico` where `Manager` = cod  and `Validità` = 'Attiva';
    commit;
END$$

DELIMITER ;

-- -----------------------------------------------------
-- View `azienda`.`proposte_terminate`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `azienda`.`proposte_terminate`;
USE `azienda`;
CREATE  OR REPLACE ALGORITHM=UNDEFINED DEFINER=`root`@`localhost` SQL SECURITY DEFINER VIEW `azienda`.`proposte_terminate` AS select `azienda`.`PropostaCommerciale`.`CodiceAlfanumerico` AS `CodiceAlfanumerico` from `azienda`.`PropostaCommerciale` where (`azienda`.`PropostaCommerciale`.`Validità` = 'Terminata');
USE `azienda`;

DELIMITER $$
USE `azienda`$$
CREATE
DEFINER=`root`@`localhost`
TRIGGER `azienda`.`Appuntamento_BEFORE_INSERT`
BEFORE INSERT ON `azienda`.`Appuntamento`
FOR EACH ROW
BEGIN
    if (select count(*) from Cliente where `CodiceFiscale` = new.PartecipazioneCliente and `IDFunz` = new.AdesioneFunzionario) = 0 then
		signal sqlstate '45000';
	end if;
END$$

USE `azienda`$$
CREATE
DEFINER=`root`@`localhost`
TRIGGER `azienda`.`Contatto_BEFORE_INSERT`
BEFORE INSERT ON `azienda`.`Contatto`
FOR EACH ROW
BEGIN 
    if (select count(*) from Cliente where `CodiceFiscale` = new.ClienteContattato and `IDFunz` = new.FunzionarioContattante) = 0 then
		signal sqlstate '45000';
	end if;
END$$

USE `azienda`$$
CREATE
DEFINER=`root`@`localhost`
TRIGGER `azienda`.`Correlato_BEFORE_INSERT`
BEFORE INSERT ON `azienda`.`Correlato`
FOR EACH ROW
BEGIN
	if (select count(*) from proposte_terminate where `CodiceAlfanumerico` = new.CodiceProposta) > 0 then
		signal sqlstate '45000';
	end if;
END$$

USE `azienda`$$
CREATE
DEFINER=`root`@`localhost`
TRIGGER `azienda`.`Nota_BEFORE_INSERT`
BEFORE INSERT ON `azienda`.`Nota`
FOR EACH ROW
BEGIN
	if (select count(*) from Contatto where `ID` = new.ContattoAssociato and `FunzionarioContattante` = new.Funzionario) = 0 then
		signal sqlstate '45000';
	end if;
END$$

USE `azienda`$$
CREATE
DEFINER=`root`@`localhost`
TRIGGER `azienda`.`Relazione_BEFORE_INSERT`
BEFORE INSERT ON `azienda`.`Relazione`
FOR EACH ROW
BEGIN
	if (select count(*) from proposte_terminate where `CodiceAlfanumerico` = new.CodiceProposta) > 0 then
		signal sqlstate '45000';
	end if;
END$$


DELIMITER ;

SET SQL_MODE=@OLD_SQL_MODE;
SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS;
SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS;
