<template>
    <b-container class="mt-3">
        <b-card>
            <h2>Raum erstellen</h2>
            <form @submit="onSubmit">
                <div class="form-group">
                    <label for="roomNumber">Raum</label>
                    <input v-model="room.number" type="text" placeholder="Raum eingeben" name="roomNumber" class="form-control">
                    <label for="dozent">Dozent</label>
                    <input v-model="room.dozent" type="text" placeholder="Name des Dozenten" name="dozent" class="form-control">
                    <label for="veranstaltung">Lehrveranstaltung</label>
                    <input v-model="room.veranstaltung" type="text" placeholder="Veranstaltung" name="veranstaltung" class="form-control">
                </div>
                <div class="form-group">
                    <button type="submit" class="btn btn-primary">Raum erstellen</button>
                </div>
            </form>

        </b-card>
    </b-container>
</template>


<script>

export default {
  name: 'Home',

  data() {
    return {
      roomNumber: '',
      dozent: '',
      veranstaltung: '',
      personenImRaum: 0,
      raumBelegt: false
    }
  },

  methods: {
    onSubmit(event) {
      event.preventDefault()
      console.log('Raum: ', this.roomNumber)
      console.log('Dozent: ', this.dozent)
      console.log('Lehrveranstaltung: ', this.veranstaltung)
      console.log('Personen im Raum: ', this.personenImRaum)
      console.log('Belegt: ', this.raumBelegt)

    }
  }
}
</script>