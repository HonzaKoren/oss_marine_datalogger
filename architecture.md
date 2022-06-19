# Logger Architecture

## Data collectors
### NMEA GPS module
'''mermaid
graph TD
    Init[Location collector] -->|Get location data| IdentRecord(Identify NMEA record type)
    IdentRecord --> ParseRecord{Parse record}
    ParseRecord -->|Location| LocationSave(fa:fa-compass Save position)
    ParseRecord -->|Time| TimeValidate{fa:fa-clock Local time invalid?}
    ParseRecord -->|Direction| DirectionSave[fa:fa-location-arrow Save direction]
    LocationSave -->|Location segment| ShMemUpdate[Publish to shmem]
    TimeValidate -->|+| SystimeUpdate(Update systime)
    SystimeUpdate -->|Time segment| ShMemUpdate(Publish to shmem)
    TimeValidate -->|-| ShMemUpdate(Publish to shmem)
    DirectionSave -->|Direction segment| ShMemUpdate[Publish to shmem]
'''
