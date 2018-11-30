require("dotenv").config();
const path = require("path");

const admin = require("firebase-admin");
// get credentials from params > service accounts
admin.initializeApp({
  credential: admin.credential.cert({
    type: process.env.TYPE,
    project_id: process.env.PJ_ID,
    private_key_id: process.env.PV_KEY_ID,
    private_key: process.env.PV_KEY,
    client_email: process.env.CLI_MAIL,
    client_id: process.env.CLI_ID,
    auth_uri: process.env.AUTH_URI,
    token_uri: process.env.TOKEN_URI,
    auth_provider_x509_cert_url: process.env.CERT_URL,
    client_x509_cert_url: process.env.CLI_CERT_URL
  }),
  databaseURL: process.env.BDD_URL
});
const db = admin.database();

// db path constants init
const DB_PATH = "chores";

module.exports = function(app) {
  app.get("/api/chores", (req, res) => {
    const LOG_STR = "GET - /api/chores - ";
    db.ref(DB_PATH).once("value", function(snapshot) {
      const dbChores = snapshot.val();
      if (dbChores) {
        const result = Object.keys(dbChores)
          .filter(key => dbChores[key].activated)
          .map(key => ({
            id: key,
            label: dbChores[key].label,
            duration: dbChores[key].duration,
            isMimi: dbChores[key].isMimi
          }));
        res.status(200).json(result);
        console.log(LOG_STR + "get all chores", result);
      } else {
        res.status(200).json({});
        console.log(LOG_STR + "no existing chore yet");
      }
    });
  });

  app.post("/api/chore", (req, res) => {
    const LOG_STR = "POST - /api/chore - ";
    const newChore = db.ref(DB_PATH).push();
    const promise = newChore.set({
      label: req.body.label,
      duration: req.body.duration,
      isMimi: req.body.isMimi,
      activated: true
    });

    promise
      .then(() => {
        db.ref(DB_PATH).once("value", function(snapshot) {
          const dbChores = snapshot.val();
          if (dbChores) {
            const result = Object.keys(dbChores)
              .filter(key => dbChores[key].activated)
              .map(key => ({
                id: key,
                label: dbChores[key].label,
                duration: dbChores[key].duration,
                isMimi: dbChores[key].isMimi
              }));
            res.status(201).json(result);
            console.log(LOG_STR + "get all chores", result);
          } else {
            res.status(201).json({});
            console.log(LOG_STR + "no existing chore yet");
          }
        });
        console.log(`${LOG_STR} ok !`);
      })
      .catch(error => {
        res.status(500).json({ message: "sorry fail." });
        console.error(LOG_STR + " error:", error);
      });
  });
  /*
  app.put("/api/journey", (req, res) => {
    const LOG_STR = "PUT - /api/journey - ";
    db.ref("journeys/" + req.body.id).set(
      {
        driverFirstName: req.body.driverFirstName,
        driverName: req.body.driverName,
        driverPhoneNumber: req.body.driverPhoneNumber,
        driverEmail: req.body.driverEmail,
        date: req.body.date,
        fromCity: req.body.fromCity,
        toCity: req.body.toCity,
        freeSeats: req.body.freeSeats,
        comment: req.body.comment,
        activated: true
      },
      error => {
        if (error) {
          res.json({
            saved: false,
            message:
              "La modification n'a pas été enregistrée, veuillez réessayer plus tard."
          });
          console.error(LOG_STR + "error updating journey:", error);
        } else {
          let message = "Modification enregistrée.";
          if (
            process.env.SEND_MAIL === "true" &&
            emailValidator.validate(req.body.driverEmail)
          ) {
            sendNewJourneyConfirmationMail(req.body, function(error) {
              if (error) {
                console.error(
                  `${LOG_STR} error sending confirmation mail to: ${
                    req.body.driverEmail
                  }, error:`,
                  error
                );
              } else {
                console.log(
                  `${LOG_STR}journey confirmation sent ${req.body.driverEmail}`
                );
                message += validMailMessage;
              }
            });
          }
          res.status(200).json({
            saved: true,
            message
          });
          console.log(`${LOG_STR}journey ${req.body.id} updated`);
        }
      }
    );
  });

  app.delete("/api/journey", (req, res) => {
    const LOG_STR = "DELETE - /api/journey - ";
    db.ref("journeys/" + req.body.id)
      .update({ activated: false })
      .then(() => {
        res.status(200).json({});
        console.log(`${LOG_STR}journey ${req.body.id} deleted`);
      })
      .catch(error => {
        res.json({
          saved: false,
          message:
            "La suppression n'a pas été enregistrée, veuillez réessayer plus tard."
        });
        console.error(
          `${LOG_STR}error deleting journey ${req.body.id}, error:`,
          error
        );
      });
  });
  */
};
