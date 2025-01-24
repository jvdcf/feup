<footer id="footer-popup" class="bg-secondary text-white py-3 position-fixed bottom-0 start-0 w-100 d-none d-flex w-100 justify-content-between">
    <div class="m-2">
        <strong>SyncTask</strong>: <small>Project management done easier, keeping everyone up to date</small>
    </div>
    <div class="m-2"> 
        <a class="text-white text-decoration-none me-3" href="{{ route('project.index') }}" >Web App</a>
        <a class="text-white text-decoration-none me-3" href="{{ route('guest.features') }}" >Features</a>
        <a class="text-white text-decoration-none me-3" href="{{ route('guest.about') }}" >About Us</a>
        <a class="text-white text-decoration-none me-3" href="{{ route('guest.contacts') }}" >Contacts</a>
    </div>
</footer>
<div class="d-flex justify-content-center">
    <button id="footer-toggle"
            class="btn btn-dark position-fixed bottom-0 m-3 rounded-circle d-flex justify-content-center align-items-center"
            style="width: 50px; height: 50px;">&#9650;
    </button>
</div>
