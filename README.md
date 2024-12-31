The project aims to simulate the spread of an epidemic within a population at the level of individual interactions.

Each day, individuals who have never been infected interact with their contacts. If they encounter infected individuals, they can contract the disease and become contagious starting the following day. While in this contagious state, individuals move freely and can spread the infection to their contacts. After an incubation period, which is the same for all individuals, the infection is detected.

For simplicity, the entire healthcare system is represented by a single hospital. At the beginning of each day, detected individuals join the hospital queue until its maximum capacity is reached. If there is enough space, all new arrivals are admitted immediately. Otherwise, a priority queue is established to determine the most urgent cases. Individuals who are not admitted to the hospital are placed in home quarantine but remain in the queue. This means that at the start of each subsequent day, they can be admitted if spaces become available and no more urgent cases are detected.

Both hospitalized and home-quarantined individuals stop infecting their contacts. They remain in their respective states or may transition from quarantine to hospitalization until they are discharged. At the beginning of each day, the number of days since detection (including the day of detection) is evaluated for every hospitalized or quarantined individual. If the sum of the days spent in quarantine plus twice the days spent in the hospital exceeds the recovery threshold, the individual is discharged or released from quarantine that same morning. If the individual was hospitalized, their discharge frees up a hospital bed immediately. Once admitted, individuals remain in the hospital until they are discharged. Discharged individuals are considered immune.

In summary, each day begins with the discharge of recovered individuals from the hospital and quarantine. Newly detected cases are then queued, and both new and previous cases are distributed between the hospital and home quarantine. Finally, new infections are evaluated, which will become contagious starting the next day.

The simulation outputs the results, including statistical data about the pandemic, to the terminal.






