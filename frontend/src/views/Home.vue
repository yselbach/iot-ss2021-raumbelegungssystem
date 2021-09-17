<template>
  <div>
    <b-container>

      <b-form @submit="onSubmit" @reset="onReset">
        <div class="form-group">
        <b-form-input v-model="roomNumber" type="text" placeholder="Raum eingeben" class="mb-2"/>
        <b-form-input v-model="dozent" type="text" placeholder="Name des Dozenten" class="mb-2"/>
        <b-form-input v-model="veranstaltung" type="text" placeholder="Lehrverantstaltung" class="mb-2"/>
        </div>
        <div class="form-group">
        <b-button type="submit" class="mr-2" variant="primary">Raum erstellen</b-button>
        <b-button type="reset" variant="danger">Eingabe löschen</b-button>
        </div>
      </b-form>
    </b-container>
  </div>
</template>

<script>
// @ is an alias to /src
//import HelloWorld from '@/components/HelloWorld.vue'



export default {
  name: 'Home',

  data() {
    return {
      roomNumber: '',
      dozent: '',
      veranstaltung: ''
    }
  },

  methods: {
    onSubmit(event) {
      event.preventDefault()
      console.log('Raum: ', this.roomNumber)
      console.log('Dozent: ', this.dozent)
      console.log('Lehrveranstaltung: ', this.veranstaltung)
    },

    onReset(event) {
      event.preventDefault()

      this.roomNumber = ''
      console.log('Eingabe zurückgesetzt!')
    }
  }
}
</script>
